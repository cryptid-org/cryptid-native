const { testComponents } = require('../common/test');
const { discoverAllComponents, removeFiles } = require('../common/util');


module.exports = {
    command: 'test [components...]',
    desc: 'Runs the tests of the specified components (separated by spaces). If no components are set, all of them will be tested.',
    handlerFactory(dependencies) {
        return function handler({ components = discoverAllComponents(dependencies) }) {
            try {
                testComponents(dependencies, components);
            } finally {
                removeFiles(dependencies, ['*.o', '*.out']);
            }
        };
    }
};
