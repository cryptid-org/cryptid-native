module.exports = {
    command: 'clean',
    desc: 'Removes all output directories and files produced by this tool.',
    handlerFactory({ fs, fg, paths }) {
        return function handler() {
            const fgOptions = {
                absolute: true,
                cwd: paths.root
            };

            const globs = [
                '**/*.o',
                '**/*.out',
                '*.gcno',
                '*.gcda'
            ];

            const files = fg.sync(globs, fgOptions);

            const directories = [
                paths.test.resultsDirectory,
                paths.coverage.root,
                paths.build.outputDirectory,
            ];

            [...files, ...directories].forEach(path => fs.removeSync(path));
        };
    }
};
