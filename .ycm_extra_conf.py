"""YCM conf"""
def FlagsForFile(*args, **kwargs):
    return {
        'flags': [
            '-x', 'c', '-I', './deps', '-Wall', 'parsing.c', 'deps/mpc/mpc.c', '-Wextra', '-Werror'
            ],
    }
