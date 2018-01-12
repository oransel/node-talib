{
    'targets': [{
        'target_name': 'talib',
        'sources': [
            'src/talib.cpp'
        ],
        "include_dirs": [
            "<!(node -e \"require('nan')\")"
        ],
        'conditions': [
            ['OS=="linux"', {
                "libraries": [
                    "../src/lib/lib/libta_abstract_csr.a",
                    "../src/lib/lib/libta_func_csr.a",
                    "../src/lib/lib/libta_common_csr.a",
                    "../src/lib/lib/libta_libc_csr.a",
                ]
            }],
            ['OS=="mac"', {
                'xcode_settings': {
                    'OTHER_CPLUSPLUSFLAGS': ['-std=c++11', '-stdlib=libc++'],
                    'OTHER_LDFLAGS': ['-stdlib=libc++'],
                    'MACOSX_DEPLOYMENT_TARGET': '10.7',
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                },
                "libraries": [
                    "../src/lib/lib/libta_abstract_csr.a",
                    "../src/lib/lib/libta_func_csr.a",
                    "../src/lib/lib/libta_common_csr.a",
                    "../src/lib/lib/libta_libc_csr.a",
                ]
            }],
            ['OS=="win"', {
                "libraries": [
                    "../src/lib/lib/ta_libc_csr.lib",
                    "../src/lib/lib/ta_func_csr.lib",
                    "../src/lib/lib/ta_common_csr.lib",
                    "../src/lib/lib/ta_abstract_csr.lib"
                ]
            }],
        ]
    }]
}
