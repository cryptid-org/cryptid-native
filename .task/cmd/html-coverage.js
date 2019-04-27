const { generateHtmlCoverage } = require('../common/coverage');
const { discoverAllComponents } = require('../common/util');


module.exports = {
    command: 'html-coverage [components...]',
    desc: 'Runs the tests of the specified components and generates human-readable HTML coverage information. If no components are set, all of them will be tested.',
    handlerFactory(dependencies) {
        return function handler({ components = discoverAllComponents(dependencies) }) {
            generateHtmlCoverage(dependencies, components);
        };
    }
};
