const { buildStaticLibrary } = require('../common/build');


module.exports = {
    command: 'build [args...]',
    desc: 'Builds a static library. Optional args will be passed to compiler (gcc default).',
    handlerFactory(dependencies) {
        return function handler({ args }) {
            var compiler;

            if(args) {
                args = args.filter(arg => {
                    if(arg.includes('compiler=')) {
                        compiler = arg.split("=")[1];
                        return false;
                    }

                    return true;
                });
            }

            buildStaticLibrary(dependencies, compiler, args);
        };
    }
};
