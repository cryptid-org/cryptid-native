const { runMemoryCheck } = require('../common/memory-check');
const { discoverAllComponents } = require('../common/util');


module.exports = {
    command: 'memory-check [components...]',
    desc: 'Runs the tests of the specified components with valgrind memory checking. If no components are set, all of them will be tested.',
    handlerFactory(dependencies) {
        return function handler({ components = discoverAllComponents(dependencies) }) {
            runMemoryCheck(dependencies, components);
        };
    }
};
