# To-do list

- Add a command line option to make the preprocessor/compiler not stop at the first error but try to recover and keep parsing
  - Specify that this can create false positives

- Add a command line option to disable phase stacking.
  - This makes the compiler run one phase at a time.
  - Better progress and debugging, much longer compilation times

- Avoid printing gigantic "affected source code" in errors when showing errors about incomplete large composite elements
- Show list of allowed elements when OneOf fails, instead of whatever the first element specified in the pattern is

- Add a "layers" button or something in the extension that shows the code colored based on different criteria or displays/hides informations
  - THIS DATA IS ON-DEMAND THROUGH COMPILER OPTIONS
  - store it in a serializable "DecoratedSource" that contains extra optional info as opposed to only the position of the character like with AnnotatedSource
  - layers:
    - Expanded macros
    - kept/optimized out
    - Executed in compile time
    - Show lines merged with LSTs
    - Hide comments
    - Replace aliases with original name
    - Nested string literals: Each level of nesting is colored differently
    - highlight implicitly converted values (can show multiple stacked levels)

- add ? and * and ** to include paths, check documentation
  - use the standard string literal parser, add special support for <>
    - write that include paths are just normal string literals with all of the supported escape sequences, but <> can be used to include standard modules
  - file names can't contain *, ?, ", <, or >
  - 0 includes if glob patterns don't match anything
  - file can't be named "." or "..
  - add a specific error in case ~ is used at the start of a non existing path (~ is not the home directory but the literal character ~)

- we need some flattening phase or something?
  - this must be done before checking the code's logic and optimization
  - this lets subsequent phases check and reference semantics easily
  - This phase must:
    - replace aliases with the proper name (and import/export renames)
    - generate specializations and separate them into different unique routines/structs/enum/whatever (this might need to be in a separate phase)
    - flatten all paths and specializations into unique names (merge namespaces)
    - collect all unique instances of each category of element into the same group




- rename "routine parameters" to "simple parameters". routine parameters should encompass all types of parameters. use a __base_ST_Parameter for that
- add error paths to syntax patterns. these take the pattern and an error code, which is then used to specify the proper "not allowed here" error if matching