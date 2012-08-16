{
    'targets': [
        {
            'target_name': 'talib',
            'sources': [
                'src/talib.cpp'
            ],
            'link_settings': {
                'libraries': [
                    '../src/lib/src/.libs/libta_lib.a'
                ]
            }
        }
    ]
}