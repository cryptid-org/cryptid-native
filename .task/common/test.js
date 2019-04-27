const { compileAllSources, compileExecutableForComponent } = require('./compile');
const { removeFiles, run } = require('./util');


function testComponents(dependencies, componentNames, extraCompilationArguments = []) {
    compileAllSources(dependencies, extraCompilationArguments);

    componentNames
        .map(name => compileExecutableForComponent(name, dependencies, extraCompilationArguments))
        .forEach(executable => {
            run(dependencies, executable);

            removeFiles(dependencies, [executable]);
        });
};

module.exports = {
    testComponents
};
