const { compileAllSources } = require('./compile');
const { removeFiles, run, version } = require('./util');

function buildStaticLibrary(dependencies, args = []) {
    try {
        compileAllSources(dependencies, args);

        if (!dependencies.fs.existsSync(dependencies.paths.build.outputDirectory)) {
            dependencies.fs.mkdirSync(dependencies.paths.build.outputDirectory);
        }

        const currentVersion = version(dependencies);
        const objectFiles = dependencies.fg.sync(['*.o'], { cwd: dependencies.paths.root });

        const arArgs = [
            'rcs',
            dependencies.paths.build.libraryFile(currentVersion),
            ...objectFiles
        ];

        run(dependencies, 'ar', arArgs, { cwd: dependencies.paths.root });
    } finally {
        removeFiles(dependencies, ['*.o']);
    }
};



module.exports = {
    buildStaticLibrary
};
