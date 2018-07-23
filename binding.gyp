{
   "targets":[
      {
         "target_name":"myAddon",
         "sources":[
            "myAddon.cc"
         ],
         "include_dirs":[
            "<!(node -e \"require('nan')\")",
             "<!@(node -p \"require('node-addon-api').include\")"
         ],
          'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      }
   ]
}