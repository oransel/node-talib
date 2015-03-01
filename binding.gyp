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
            'link_settings': {
                'libraries': [
                    '../src/lib/lib/libta_libc_csr.a'
                ]
            }
        }
    ]
}