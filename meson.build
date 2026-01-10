project('Twikr', 'cpp', default_options : ['cpp_std=c++20'])

# Documentation: https://mesonbuild.com/Qt5-module.html
qt = import('qt6')
qtdep = dependency('qt6', modules : ['Core', 'Widgets'])

add_global_arguments('-Wno-ignored-attributes', language : 'cpp')
# add_global_arguments('-Wno-nonportable-include-path', language : 'cpp')

generated_files = qt.preprocess(
                            moc_headers : ['mainwidget.h', 'engine.h', 'colorbutton.h'],
                            ui_files : 'mainwidget.ui',
                            dependencies: [qtdep])

# application icon
windows = import('windows')
res = windows.compile_resources('resources.rc')

executable('Twikr'
        , 'main.cpp'
        , 'mainwidget.cpp'
        , 'engine.cpp'
        , 'colorbutton.cpp'
        , generated_files
        , res
        , dependencies : [qtdep]
        , install : true
        , link_args : ['-static-libgcc', '-static-libstdc++'])

