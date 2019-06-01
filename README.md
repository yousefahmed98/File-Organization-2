# File-Organization-2
we want to save 10 courses.
Saveing the data for the courses in the following format: delimited fields, length indicator records.
developing the following indexes
1. Primary index using the course ID
2. Secondary index using instructor name
Important notes:
  all indexes sorted ascending
 there are status flag to check that indexes are up-to-date.
 Implement the secondary index using inverted list technique.
 Searching in indexes is performed using binary search.
 To delete a record just put an * at the beginning of that record. (no need for avail list implementation) it's on another programme
 the operations (add, delete , update) will affect indexes.
 An instructor may have more than one course, in case of deletion by instructor name, delete all courses by that instructor.
 Search operations will use indexes (primary or secondary)
 Bind secondary index with the primary index, don’t bind it by offset directly.
