/*
* Copyright (C) 2010 The Android Open Source Project
*
* Licenciado sob Apache License, Vers�o 2.0 (a "Licen�a");
* voc� n�o pode usar esse arquivo, exceto se estiver em conformidade com a Licen�a.
* Voc� pode obter uma c�pia da Licen�a em
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* A menos que solicitado pela lei aplic�vel ou acordado por escrito, o software
* distribu�do sob a Licen�a � distribu�do em uma BASE ''COMO EST�'',
* SEM GARANTIAS OU CONDI��ES DE NENHUM TIPO, expressas ou impl�citas.
* Consulte o idioma espec�fico governando as permiss�es e
* limita��es sob a Licen�a.
*
*/

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))

/**
* Nossos dados de estado salvos.
*/
struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

/**
* Estado compartilhado do nosso aplicativo.
*/
struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	struct saved_state state;
};

/**
* Inicializar um contexto EGL para a exibi��o atual.
*/
static int engine_init_display(struct engine* engine) {
	// inicializar OpenGL ES e EGL

	/*
	* Aqui, especifica os atributos da configura��o desejada.
	* Abaixo, selecionamos um EGLConfig com pelo menos 8 bits por componente de cor
	* compat�vel com janelas na tela
	*/
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Aqui, o aplicativo escolhe a configura��o que desejar. Neste
	 * modelo, temos um processo de sele��o muito simplificado, em que podemos escolher
	* o primeiro EGLConfig que atenda aos nossos crit�rios */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID � um atributo de EGLConfig que est�
	* garantido para ser aceito por ANativeWindow_setBuffersGeometry().
	* Assim que escolhermos um EGLConfig, ser� poss�vel reconfigurar os buffers de
	* ANativeWindow para corresponder, usando EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->state.angle = 0;

	// Inicializar estado GL.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

	return 0;
}

/**
* Somente o quadro atual na tela.
*/
static void engine_draw_frame(struct engine* engine) {
	if (engine->display == NULL) {
		// Sem tela.
		return;
	}

	// Basta preencher a tela com uma cor.
	glClearColor(((float)engine->state.x) / engine->width, engine->state.angle,
		((float)engine->state.y) / engine->height, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	eglSwapBuffers(engine->display, engine->surface);
}

/**
* Desative o contexto de EGL atualmente associado � tela.
*/
static void engine_term_display(struct engine* engine) {
	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->animating = 0;
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

/**
* Processe o pr�ximo evento de entrada.
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	struct engine* engine = (struct engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		engine->state.x = AMotionEvent_getX(event, 0);
		engine->state.y = AMotionEvent_getY(event, 0);
		return 1;
	}
	return 0;
}

/**
* Processe o pr�ximo comando principal.
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		// O sistema pediu para salvar nosso estado atual.  Fa�a-o.
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;
	case APP_CMD_INIT_WINDOW:
		// A janela est� sendo exibida, prepare-a.
		if (engine->app->window != NULL) {
			engine_init_display(engine);
			engine_draw_frame(engine);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// A janela est� sendo ocultada ou fechada, limpe-a.
		engine_term_display(engine);
		break;
	case APP_CMD_GAINED_FOCUS:
		// Quando nosso aplicativo obtiver foco, come�aremos a monitorar o aceler�metro.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
			// Gostar�amos de obter 60 eventos por segundo (nos EUA).
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
				engine->accelerometerSensor, (1000L / 60) * 1000);
		}
		break;
	case APP_CMD_LOST_FOCUS:
		// Quando nosso aplicativo perder foco, paramos de monitorar o aceler�metro.
		// Isso � para evitar o consumo da bateria enquanto ela n�o est� sendo usada.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
		}
		// Tamb�m interromper anima��o.
		engine->animating = 0;
		engine_draw_frame(engine);
		break;
	}
}

/**
* Este � o ponto de entrada principal de um aplicativo nativo usando
 * android_native_app_glue.  Ele � executado em seu thread, com seu pr�prio loop de eventos
* para receber eventos de entrada e outros itens.
*/
void android_main(struct android_app* state) {
	struct engine engine;

	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;

	// Preparar para monitorar aceler�metro
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
		state->looper, LOOPER_ID_USER, NULL, NULL);

	if (state->savedState != NULL) {
		// Estamos come�ando com um estado anterior salvo; fa�a a restaura��o por meio dele.
		engine.state = *(struct saved_state*)state->savedState;
	}

	engine.animating = 1;

	// loop aguardando itens.

	while (1) {
		// Leia todos os eventos pendentes.
		int ident;
		int events;
		struct android_poll_source* source;

		// Se n�o estiver animando, bloquearemos indefinidamente, aguardando eventos.
		// Se estiver animado, faremos um loop at� que todos os eventos sejam lidos e continuaremos
		// para desenhar o pr�ximo quadro da anima��o.
		while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// Processar este evento.
			if (source != NULL) {
				source->process(state, source);
			}

			// Se um sensor tiver dados, processe-o agora.
			if (ident == LOOPER_ID_USER) {
				if (engine.accelerometerSensor != NULL) {
					ASensorEvent event;
					while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
						&event, 1) > 0) {
						LOGI("accelerometer: x=%f y=%f z=%f",
							event.acceleration.x, event.acceleration.y,
							event.acceleration.z);
					}
				}
			}

			// Verifique se estamos saindo.
			if (state->destroyRequested != 0) {
				engine_term_display(&engine);
				return;
			}
		}

		if (engine.animating) {
			// Conclu�do com eventos; desenhe o pr�ximo quadro de anima��o.
			engine.state.angle += .01f;
			if (engine.state.angle > 1) {
				engine.state.angle = 0;
			}

			// O desenho � acelerado para a taxa de atualiza��o de tela, para que
			// n�o seja necess�rio fazer o intervalo aqui.
			engine_draw_frame(&engine);
		}
	}
}
