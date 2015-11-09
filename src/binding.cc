#include <nan.h>
#include "SpiceUsr.h"

namespace NodeSpice {

    using namespace v8;

    #define MSG_LEN 1024

    bool checkForCSPiceError() {
        if (failed_c()) {
            SpiceChar msg[MSG_LEN];
            getmsg_c("SHORT", MSG_LEN, msg);
            Nan::ThrowTypeError(msg);
            return true;
        }
        return false;
    }

    void furnsh(const Nan::FunctionCallbackInfo<Value>& info) {
        if (info.Length() < 1 || !info[0]->IsString()) {
            Nan::ThrowTypeError("a kernel file is required");
            return;
        }

        furnsh_c(*String::Utf8Value(info[0]->ToString()));
    }

    void spkezr(const Nan::FunctionCallbackInfo<Value>& info) {
        if (info.Length() != 4) {
            Nan::ThrowTypeError("observer, target, epoch and reference frame arguments are required");
            return;
        }

        String::Utf8Value observer(info[0]->ToString());
        String::Utf8Value target(info[1]->ToString());
        String::Utf8Value frame(info[3]->ToString());

        SpiceDouble epoch;
        str2et_c(*String::Utf8Value(info[2]->ToString()), &epoch);
        if (checkForCSPiceError()) {
            return;
        }

        //printf("\nepoch = %20.10f\n", epoch);

        SpiceDouble lt;
        SpiceDouble state [6];
        spkezr_c(*target, epoch, *frame, "NONE", *observer, state, &lt);

        Local<Array> arr = Nan::New<Array>(6);
        arr->Set(0, Nan::New<Number>(state[0]));
        arr->Set(1, Nan::New<Number>(state[1]));
        arr->Set(2, Nan::New<Number>(state[2]));
        arr->Set(3, Nan::New<Number>(state[3]));
        arr->Set(4, Nan::New<Number>(state[4]));
        arr->Set(5, Nan::New<Number>(state[5]));

        info.GetReturnValue().Set(arr);
    }

    void exportFunction(Handle<Object> exports, char* name, void(*function)(const Nan::FunctionCallbackInfo<Value>& info)) {
        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(function);
        Local<Function> fn = tpl->GetFunction();
        Local<String> n = Nan::New(name).ToLocalChecked();
        fn->SetName(n);
        exports->Set(n, fn);
    }

    void init(Handle<Object> exports) {
        // disable CSPICE automatic output of errors to stdout
        errprt_c("SET", MSG_LEN, "NONE");
        // disable CSPICE automatic halting of programs
        erract_c("SET", MSG_LEN, "RETURN");

        exportFunction(exports, "furnsh", furnsh);
        exportFunction(exports, "spkezr", spkezr);
    }

    NODE_MODULE(addon, init)
}
