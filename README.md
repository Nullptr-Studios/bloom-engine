# Bloom engine
This is the bloom engine!

## Coding conventions
### Naming conventions
- Namespaces use `snake_case`
- Classes and Functions use `PascalCase`
- Variables use `camelCase`
  - Private and Protected variables must be prefixed with `m_variableName`

### Code style
- NO TABS, 2 spaces
- Soft line limit of 120 characters
- Hard line limit of 150 characters
- Use `#pragma region` to group functions inside source files if there are a lot of functions
- Favour longer classes
- Favour inheritance over ECS

### Documentation
EVERY public namespace, class, function and variable should be documented. Private members aren't necessary but highly recomended for complex functions. Doxygen prefix `@` is prefered over `\`. A good documentation must have a `@brief` explanation and a more detailed one. Comments explaining code should end with a sufix indicating who wrote the comment.
```
-x -> Xein
-m -> Dario
-d -> Dante
-a -> Alexey
-t -> Tommasso
```
For code that needs to be reviewed or for notes that are EXTREMELLY important use TODO for easy tracking. Completed tasks should be deleted after.
```
// TODO: your note here
```
Header files must have a header comment that looks like this: 
```cpp
/**
 * @file {FILE_NAME}.hpp
 * @author {NAME} <{EMAIL}>
 * @date {DATE(DD/MM/YYYY)}
 *
 * @brief [Brief description of the file's purpose]
 */
``` 

### Git