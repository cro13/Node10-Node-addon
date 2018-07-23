#define NAPI_VERSION 3
#include <napi.h>

napi_value concat(napi_env env, const napi_callback_info info) {
  napi_status status;
  char* value1;
  char* value2;
  size_t argc = 2;
  napi_value argv[2];

  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to parse arguments");
  }

  // Throw custom error type
  if (argc < 2) {
    napi_throw_type_error(env, "ERR_INVALID_ARG_TYPE", "Wrong number of arguments");
    return nullptr;
  }

  // Extract the length of the first string.
  size_t firstLength;
  napi_get_value_string_utf8(env, argv[0], NULL, 0, &firstLength);
  value1 = new char[firstLength +1];

  // Extract the value of the first string
  status = napi_get_value_string_utf8(env, argv[0], value1, firstLength + 1, 0);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Something went wrong.");
  }

  // Extract the length of the second string
  size_t secondLength;
  napi_get_value_string_utf8(env, argv[1], NULL, 0, &secondLength);
  value2 = new char[secondLength +1];

  // Extract the value of the second string
  status = napi_get_value_string_utf8(env, argv[1], value2, secondLength + 1, 0);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Something went wrong.");
  }

  strcat(value1, value2); // char * strcat ( char * destination, const char * source );
  napi_value result;
  size_t totalLength = firstLength + secondLength;
  status = napi_create_string_utf8(env, value1, totalLength, &result);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Something went wrong.");
  }

  return result;
}

void asyncSum(napi_env env, napi_deferred deferred) {
  napi_status status;
  napi_value result;

  // Create a value with which to conclude the deferred.
  status = napi_create_int32(env, 42, &result);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Something went wrong.");
  }

  // Resolve or reject the promise associated with the deferred depending on
  // whether the asynchronous action succeeded.
  if (true) {
    status = napi_resolve_deferred(env, deferred, result);
    }
    else  {
      status = napi_reject_deferred(env, deferred, result);
    }

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Something went wrong.");
  }

  // At this point the deferred has been freed, so we should assign NULL to it.
  deferred = NULL;
}

napi_value sumPromise(napi_env env, const napi_callback_info info) {
  napi_status status;

  napi_deferred deferred;
  napi_value promise;

  status = napi_create_promise(env, &deferred, &promise);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Something went wrong.");
  }

  asyncSum(env, deferred);

  return promise;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value fn;

  status = napi_create_function(env, NULL, 0, concat, NULL, &fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to wrap native function");
  }

  status = napi_set_named_property(env, exports, "concat", fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to populate exports");
  }

  status = napi_create_function(env, NULL, 0, sumPromise, NULL, &fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to wrap native function");
  }

  status = napi_set_named_property(env, exports, "sumPromise", fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to populate exports");
  }

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)