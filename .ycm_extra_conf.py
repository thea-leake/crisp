"""YCM conf"""
def FlagsForFile(*args, **kwargs):
    return {
        'flags': [
            '-x', 'c', 'std', 'c99', '-I', './deps', '-Wall', 'parsing.c', 'deps/mpc/mpc.c', '-Wextra', '-Werror'
            ],
    }
