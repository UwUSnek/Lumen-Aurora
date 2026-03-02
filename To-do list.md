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
  - we need something like this for syntaxes and suggestions / warnings.
  - writing stuff that doesn't exist in this language but works in C? tree creation phase should be able to parse it and print an error + suggestion
    - writing "default" as a case in a match statement
    - writing "elif" or "elseif" after an if statement
    - using "class" or "typename" in a template parameter

- statement expressions
  @<statement>, result value is whatever gets yielded first
  - while, do-while, for, if, match. includes chained else statements

- REMOVE EXPRESSION STATEMENTS
  - replace them with routine call statements
    - though they could just be warnings. also add the disclaimer in the expression statement page?
    - "value is discarded (@if(n) f();). This can be replaced with if(n) f();"
  - raw expressions such as "2" or "try { ... }" whose result value is discarded are completely pointless and likely indicate a bug in the code.
    - no point in keeping them as no-ops like C does. just print an error. anything with side-effects is either an operator call or a function call.
    - @<statement> expressions could technically be used as non-call-expressions with side-effects, but at that point, just write a normal loop or smt.

- try expression: Compiles a portion of code using the same options as the parent module. Evaluates to true if the code doesn't generate errors.
  - template<t> void f(t n) {
      bool r = test { n.inc(); }
      if(!r) //qualche cosa per un errore compile time
    }

- Check if meta keywords actually work
- Check if meta keywords conflict with keywords with the same name. They are meant to have priority over keywords when in dedicated places