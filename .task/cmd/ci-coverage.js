const { generateGcovCoverage } = require('../common/coverage');
const { discoverAllComponents } = require('../common/util');


module.exports = {
    command: 'gcov-coverage [components...]',
    desc: 'Runs the tests of selected components and generates gcov coverage information. If no components are set, all of them will be tested.',
    handlerFactory(dependencies) {
        return function handler({ components = discoverAllComponents(dependencies) }) {
            generateGcovCoverage(dependencies, components);
        };
    }
};
