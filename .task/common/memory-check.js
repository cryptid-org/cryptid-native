const { compileAllSources, compileExecutableForComponent } = require('./compile');
const { removeFiles, run } = require('./util');


function runMemoryCheck(dependencies, components, xmlOutput) {
    try {
        compileAllSources(dependencies, ['-g']);

        const errors = testWithMemoryCheck(dependencies, components, xmlOutput);

        if (errors.length > 1) {
            console.log(errors);
            throw new Error('There were errors during the memory check. Please see the log lines above.');
        }
    } finally {
        removeFiles(dependencies, ['*.o', '*.out']);
    }
};

function testWithMemoryCheck(dependencies, components, xmlOutput) {
    const errors = [];

    for (const component of components) {
        console.log(`Running memory checks for ${component}`);

        const executable = compileExecutableForComponent(component, dependencies, []);

        const valgrindOptions = [
            '--leak-check=full',
            '--show-reachable=yes',
            '--error-exitcode=1'
        ]

        if (xmlOutput) {
            try {
                dependencies.fs.removeSync(dependencies.paths.memcheck.root);
                dependencies.fs.mkdirSync(dependencies.paths.memcheck.root);
            } catch (e) {
                // Calculated
            }

            valgrindOptions.push(...[
                '--xml=yes',
                `--xml-file=${dependencies.paths.memcheck.componentMemcheckFile(component)}`,
                '--child-silent-after-fork=yes'
            ]);
        }

        valgrindOptions.push(executable);

        if (component == 'CryptID' || component == 'SignID') {
            valgrindOptions.push('--', '--lowest-quick-check');
        }

        try {
            run(dependencies, 'valgrind', valgrindOptions, { cwd: dependencies.paths.root });

            removeFiles(dependencies, [executable]);
        } catch (e) {
            errors.push({
                component,
                error: e
            });
        } finally {
            removeFiles(dependencies, [executable]);
        }

        console.log('\n');
    }

    return errors;
}

module.exports = {
    runMemoryCheck
};
