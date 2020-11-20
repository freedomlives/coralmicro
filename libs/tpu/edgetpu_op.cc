#include "libs/tpu/edgetpu_op.h"
#include "libs/tpu/edgetpu_manager.h"
#include "third_party/tensorflow/tensorflow/lite/c/common.h"

namespace valiant {

static void* CustomOpInit(TfLiteContext *context, const char *buffer, size_t length) {
    return EdgeTpuManager::GetSingleton()->RegisterPackage(buffer, length);
}

static void CustomOpFree(TfLiteContext *context, void *buffer) {}

static TfLiteStatus CustomOpPrepare(TfLiteContext *context, TfLiteNode *node) {
    if (node->user_data == nullptr) {
        return kTfLiteError;
    }
    return kTfLiteOk;
}

static TfLiteStatus CustomOpInvoke(TfLiteContext *context, TfLiteNode *node) {
    EdgeTpuPackage* package = (EdgeTpuPackage*)node->user_data;
    return EdgeTpuManager::GetSingleton()->Invoke(package, context, node);
}

TfLiteRegistration* RegisterCustomOp() {
    static TfLiteRegistration registration = {
        CustomOpInit,
        CustomOpFree,
        CustomOpPrepare,
        CustomOpInvoke,
    };
    return &registration;
}

}  // namespace valiant
