{
  "targets": [
    {
      "target_name": "memkitty.node",

      "sources": [
          "native/addon.cpp",
          "native/native-process/native-process.cpp",
          "native/process/process.cpp",
          "native/memory/memory.cpp",
          "native/process-utils/process-utils.cpp",
          "native/native-process/native-process-validator.cpp"
      ],

      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],

      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],

      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS"
      ],

      "conditions": [
        [
          "OS=='win'",
          {
            "defines": [
              "_WIN32_WINNT=0x0601"
            ]
          }
        ]
      ]
    }
  ]
}