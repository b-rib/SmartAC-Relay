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
 * distribu�do sob a licen�a � distribu�do em uma BASE ''COMO EST�'',
 * SEM GARANTIAS OU CONDI��ES DE NENHUM TIPO, expressas ou impl�citas.
 * Consulte a Licen�a para permiss�es de governo espec�ficas e
 * limita��es sob a Licen�a.
 *
 */

#ifndef _ANDROID_NATIVE_APP_GLUE_H
#define _ANDROID_NATIVE_APP_GLUE_H

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A interface da atividade nativa fornecida por <android/native_activity.h>
 * baseia-se em um conjunto de retornos de chamada fornecidos pelo aplicativo que ser� chamado
 * pelo thread principal da Atividade quando determinados eventos ocorrerem.
 *
 * Isso significa que cada um desses retornos de chamada n�o deve bloquear ou
 * arriscam-se a ter o fechamento for�ado pelo sistema do aplicativo. Este modelo de programa��o
 * � direto, leve, mas tem restri��es.
 *
 * A biblioteca est�tica 'threaded_native_app' � usada para fornecer um modelo de execu��o
 * diferente em que o aplicativo pode implementar seu pr�prio loop de evento principal
 * em um thread diferente. Veja aqui como funciona:
 *
 * 1/ O aplicativo deve fornecer uma fun��o nomeada "android_main()" que
 *    ser� chamada quando a atividade for criada, em um novo thread
 *    distinto do thread principal da atividade.
 *
 * 2/ android_main() recebe um ponteiro para uma estrutura "android_app" v�lida
 *    que cont�m refer�ncias a outros objetos importantes, por exemplo, a
 *    inst�ncia de objeto ANativeActivity na qual o aplicativo � executado.
 *
 * 3/ o objeto "android_app" ret�m uma inst�ncia do ALooper que j�
 *    ouve dois itens importantes:
 *
 *      - eventos de ciclo de vida da atividade (ex.: "pausar", "retomar"). Consulte declara��es de APP_CMD_XXX
 *        abaixo.
 *
 *      - eventos de entrada provenientes de AInputQueue anexados � atividade.
 *
 *    Cada um deles corresponde a um identificador de ALooper retornado por
 *    ALooper_pollOnce com valores de LOOPER_ID_MAIN e LOOPER_ID_INPUT,
 *    respectivamente.
 *
 *    Seu aplicativo pode usar o mesmo ALooper para ouvir os
 *    descritores de arquivo adicionais.  Eles podem fazer retorno de chamada com base ou com retorno em
 *    identificadores come�ando com LOOPER_ID_USER.
 *
 * 4/ Sempre que voc� receber um evento LOOPER_ID_MAIN ou LOOPER_ID_INPUT,
 *    os dados retornados apontar�o para uma estrutura android_poll_source.  Voc�
 *    pode chamar a fun��o do processo nele e preencher android_app->onAppCmd
 *    e android_app->onInputEvent para ser chamado para seu pr�prio processamento
 *    do evento.
 *
 *    Se preferir, � poss�vel chamar fun��es de n�veis inferiores para ler e processar
 *    os dados diretamente...  observe as implementa��es de processo process_cmd() e process_input()
 *    na associa��o para ver como fazer isso.
 *
 * Consulte o modelo nomeado ''atividade nativa'' fornecido com o NDK com um
 * exemplo de uso completo.  Procure tamb�m o JavaDoc de NativeActivity.
 */

struct android_app;

/**
 * Dados associados a um ALooper fd que ser�o retornados como o ''outData"
 * quando a fonte j� tiver dados prontos.
 */
struct android_poll_source {
    // O identificador desta fonte.  Pode ser LOOPER_ID_MAIN ou
    // LOOPER_ID_INPUT.
    int32_t id;

    // O android_app ao qual este ident est� associado.
    struct android_app* app;

    // Fun��o a ser chamada para executar o processamento padr�o de dados a partir
    // dessa fonte.
    void (*process)(struct android_app* app, struct android_poll_source* source);
};

/**
 * Essa � a interface para o c�digo de associa��o de um aplicativo do
 * thread.  Nesse modelo, o c�digo do aplicativo � executado em seu pr�prio
 * thread separado do thread principal do processo.
 * N�o � necess�rio que esse thread seja associado ao VM do Java,
 * apesar de precisar para fazer chamadas de JNI para quaisquer
 * objetos Java.
 */
struct android_app {
    // O aplicativo pode inserir um ponto a seu pr�prio projeto de estado
    // aqui, se desejar.
    void* userData;

    // Preencha com a fun��o para processar os principais comandos do aplicativo (APP_CMD_*)
    void (*onAppCmd)(struct android_app* app, int32_t cmd);

    // Preencha com a fun��o para processar os eventos de entrada. Nesse ponto,
    // o evento j� foi pr�-expedido e ser� conclu�do ap�s
    // o retorno.  Retornar 1 se voc� manuseou o evento, 0 para qualquer expedi��o
    // padr�o.
    int32_t (*onInputEvent)(struct android_app* app, AInputEvent* event);

    // A inst�ncia do objeto ANativeActivity na qual este aplicativo est� executando.
    ANativeActivity* activity;

    // A configura��o atual na qual o aplicativo est� executando.
    AConfiguration* config;

    // Este � o �ltimo estado de salvamento da inst�ncia, como fornecido no momento da cria��o.
    // � NULO se n�o houver nenhum estado.  Voc� pode us�-lo conforme necess�rio; a
    // mem�ria permanecer� at� que voc� chame android_app_exec_cmd() para
    // APP_CMD_RESUME, no ponto em que ser� liberado e savedState definido para NULO.
    // Essas vari�veis devem ser alteradas somente ao processar um APP_CMD_SAVE_STATE,
    // no ponto em que ser�o inicializados para NULO e voc� pode fazer malloc de seu
    // estado e colocar as informa��es aqui.  Nesse caso, a mem�ria ser�
    // liberada posteriormente.
    void* savedState;
    size_t savedStateSize;

    // O ALooper associado ao thread desse aplicativo.
    ALooper* looper;

    // Quando n�o for NULO, esta � a fila de entrada da qual o aplicativo
    // receber� eventos de entrada do usu�rio.
    AInputQueue* inputQueue;

    // Quando for n�o NULO, essa � a superf�cie da janela na qual o aplicativo pode desenhar.
    ANativeWindow* window;

    // Ret�ngulo de conte�do atual da janela; esta � a �rea em que o
    // conte�do da janela deve ser colocado para ser visto pelo usu�rio.
    ARect contentRect;

    // Estado atual da atividade do aplicativo.  Pode ser APP_CMD_START,
    // APP_CMD_RESUME, APP_CMD_PAUSE ou APP_CMD_STOP; consulte abaixo.
    int activityState;

    // Isso � diferente de zero quando o NativeActivity do aplicativo �
    // destru�do e aguardando o thread de aplicativos ser conclu�do.
    int destroyRequested;

    // -------------------------------------------------
    // Abaixo est� implementa��o ''particular'' do c�digo de associa��o.

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    int msgread;
    int msgwrite;

    pthread_t thread;

    struct android_poll_source cmdPollSource;
    struct android_poll_source inputPollSource;

    int running;
    int stateSaved;
    int destroyed;
    int redrawNeeded;
    AInputQueue* pendingInputQueue;
    ANativeWindow* pendingWindow;
    ARect pendingContentRect;
};

enum {
    /**
     * ID de dados do Looper de comandos provenientes do thread principal do aplicativo, que
     * � retornado como um identificador de ALooper_pollOnce().  O dado para este
     * identificador � um ponteiro para uma estrutura android_poll_source.
     * Eles podem ser recuperados e processados com android_app_read_cmd()
     * e android_app_exec_cmd().
     */
    LOOPER_ID_MAIN = 1,

    /**
     * ID de dados do Looper provenientes da AInputQueue da
     * janela de aplicativos, que retorna como um identificador de
     * ALooper_pollOnce().  O dado para este identificador � um ponteiro para uma estrutura
     * android_poll_source.  Eles podem ser lidos pelo objeto inputQueue
     * de android_app.
     */
    LOOPER_ID_INPUT = 2,

    /**
     * In�cio de identificadores de ALooper definidos pelo usu�rio.
     */
    LOOPER_ID_USER = 3,
};

enum {
    /**
     * Comando do thread principal: o AInputQueue foi alterado.  Ap�s processar
     * esse comando, android_app->inputQueue ser� atualizado para a nova fila
     * (ou NULO).
     */
    APP_CMD_INPUT_CHANGED,

    /**
     * Comando do thread principal: um novo ANativeWindow est� pronto para uso.  Ap�s
     * receber este comando, android_app->window conter� a nova superf�cie de
     * janela.
     */
    APP_CMD_INIT_WINDOW,

    /**
     * Comando do thread principal: a ANativeWindow existente precisa ser
     * terminada.  Ao receber esse comando, android_app->window ainda
     * cont�m a janela existente; ap�s chamar android_app_exec_cmd
 * ele ser� definido como NULO.
     */
    APP_CMD_TERM_WINDOW,

    /**
     * Comando do thread principal: o ANativeWindow atual foi redimensionado.
     * Redesenhe com esse novo tamanho.
     */
    APP_CMD_WINDOW_RESIZED,

    /**
     * Comando do thread principal: o sistema precisa que o ANativeWindow atual
     * seja redesenhado.  Voc� deve redesenhar a janela antes de passar para
     * android_app_exec_cmd() para evitar falhas de desenho transit�rias.
     */
    APP_CMD_WINDOW_REDRAW_NEEDED,

    /**
     * Comando do thread principal: a �rea de conte�do da janela foi alterada,
     * como na janela de entrada simples que est� sendo exibida ou ocultada.  Voc� pode
     * encontrar o novo rect. de conte�do em android_app::contentRect.
     */
    APP_CMD_CONTENT_RECT_CHANGED,

    /**
     * Comando do thread principal: a janela de atividade do aplicativo ganhou
     * foco de entrada.
     */
    APP_CMD_GAINED_FOCUS,

    /**
     * Comando do thread principal: a janela de atividade do aplicativo perdeu
     * foco de entrada.
     */
    APP_CMD_LOST_FOCUS,

    /**
     * Comando do thread principal: a configura��o do dispositivo atual foi alterada.
     */
    APP_CMD_CONFIG_CHANGED,

    /**
     * Comando do thread principal: o sistema est� com mem�ria baixa.
     * Tente reduzir o uso da mem�ria.
     */
    APP_CMD_LOW_MEMORY,

    /**
     * Comando do thread principal: a atividade do aplicativo foi iniciada.
     */
    APP_CMD_START,

    /**
     * Comando do thread principal: a atividade do aplicativo foi retomada.
     */
    APP_CMD_RESUME,

    /**
     * Comando do thread principal: o aplicativo deve gerar um novo estado salvo
     * para si, a fim de restaurar posteriormente, se necess�rio.  Se voc� tiver o estado salvo,
     * aloque com o malloc e coloque no android_app.savedState com
     * o tamanho em android_app.savedStateSize.  Ele ser� liberado para voc�
     * posteriormente.
     */
    APP_CMD_SAVE_STATE,

    /**
     * Comando do thread principal: a atividade do aplicativo foi pausada.
     */
    APP_CMD_PAUSE,

    /**
     * Comando do thread principal: a atividade do aplicativo foi interrompida.
     */
    APP_CMD_STOP,

    /**
     * Comando do thread principal: a atividade do aplicativo est� sendo destru�da,
     * e aguardando que o thread do aplicativo fa�a a limpeza e saia antes de prosseguir.
     */
    APP_CMD_DESTROY,
};

/**
 * Chamar quando ALooper_pollAll() retornar LOOPER_ID_MAIN, lendo a pr�xima
 * mensagem de comando do aplicativo.
 */
int8_t android_app_read_cmd(struct android_app* android_app);

/**
 * Chamada com o comando retornado pelo android_app_read_cmd() para fazer o
 * pr�-processamento inicial do comando fornecido.  Voc� pode executar suas pr�prias
 * a��es para o comando ap�s chamar esta fun��o.
 */
void android_app_pre_exec_cmd(struct android_app* android_app, int8_t cmd);

/**
 * A chamada com o comando retornado pelo android_app_read_cmd() para fazer o
 * p�s-processamento final do comando fornecido.  Voc� deve ter feito suas pr�prias
 * a��es para o comando antes de chamar essa fun��o.
 */
void android_app_post_exec_cmd(struct android_app* android_app, int8_t cmd);

/**
 * Esta � a fun��o que o c�digo do aplicativo deve implementar, representando
 * a entrada principal para o aplicativo.
 */
extern void android_main(struct android_app* app);

#ifdef __cplusplus
}
#endif

#endif /* _ANDROID_NATIVE_APP_GLUE_H */
