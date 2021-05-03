//
// pch.h
// Cabe�alho para os arquivos de sistema padr�o.
//
// Usado pelo sistema de compila��o para gerar o cabe�alho pr�-compilado. Observe que nenhum
// pch.cpp necess�rio e pch.h inclu�do automaticamente em todos os arquivos cpp
// que fazem parte do objeto
//

#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>

#include <android/log.h>
#include "android_native_app_glue.h"
