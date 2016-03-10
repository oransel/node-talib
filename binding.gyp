{
    'targets': [
        {
            'target_name': 'talib',
            'sources': [
                'src/talib.cpp'
            ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ],
            'conditions': [
              [ 'OS!="win"', {
                'link_settings': {
                    "libraries" : [
                      '../src/lib/lib/libta_libc_csr.a'
                    ]
                }
              }],
              [ 'OS=="win"', {
                "libraries" : [
                   "../src/lib/lib/ta_libc_csr.lib",
                   "../src/lib/lib/ta_func_csr.lib",
                   "../src/lib/lib/ta_common_csr.lib",
                   "../src/lib/lib/ta_abstract_csr.lib"
                ]
              }],
            ]
        }
    ]
}
