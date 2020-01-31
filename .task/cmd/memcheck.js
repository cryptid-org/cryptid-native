const { runMemoryCheck } = require('../common/memory-check');
const { discoverAllComponents } = require('../common/util');


module.exports = {
    command: 'memcheck [components...]',
    desc: 'Runs the tests of the specified components with valgrind memory checking. If no components are set, all of them will be tested.',
    builder: {
        xmlOutput: {
            desc: 'Whether to write the memcheck output into an XML file (component results are written into separate files).',
            type: 'boolean',
            default: false
        }
    },
    handlerFactory(dependencies) {
        return function handler({ components = discoverAllComponents(dependencies), xmlOutput }) {
            runMemoryCheck(dependencies, components, xmlOutput);
        };
    }
};
