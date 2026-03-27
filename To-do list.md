# To-do list

- Add a command line option to make the preprocessor/compiler not stop at the first error but try to recover and keep parsing
  - Specify that this can create false positives

- Add a command line option to disable phase stacking.
  - This makes the compiler run one phase at a time.
  - Better progress and debugging, much longer compilation times

- Avoid printing gigantic "affected source code" in errors when showing errors about incomplete large composite elements

- Add a "layers" button or something in the extension that shows the code colored based on different criteria or displays/hides informations
  - kept/optimized out
  - Executed in compile time
  - Show lines merged with LSTs
  - Hide comments
  - Replace aliases with original name
