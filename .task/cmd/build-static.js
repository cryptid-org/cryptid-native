const { buildStaticLibrary } = require('../common/build');


module.exports = {
    command: 'build-static [args...]',
    desc: 'Builds a static library. Optional args will be passed to gcc.',
    handlerFactory(dependencies) {
        return function handler({ args }) {
            buildStaticLibrary(dependencies, args);
        };
    }
};
