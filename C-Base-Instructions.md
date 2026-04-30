# C-Base — Hierarchical Record Store

## Architecture
C-Base is a module under the WireLink framework.
Type: Hierarchical Record Store (tree-structured document DB).
All data mirrors in RAM and disk simultaneously.

## Hierarchy
```
CBase → Node → Table → Item → Cell
```
- **Node** = folder (directory on disk)
- **Table** = file inside node (.table + .map files)
- **Item** = one line inside table file (one record)
- **Cell** = key:value pair inside item (parsed by C-Text/CORDA)

## Dependencies
- **C-Text (CORDA)** — parses `key:value;` format strings inside Items
- **filemanager.h** — disk read/write operations

## Construction Pattern
Parent passed in constructor, auto-registers child:
```cpp
Node school("school");              // root node, creates directory
Node dept("CS", school);            // child node, creates school/CS/
Table students("students", dept);   // creates school/CS/students.table + .map
Item ali("ali", students);          // inserted into students table
```

## Disk Layout
```
school/                    ← Node (directory)
  CS/                      ← Node (directory)
    students.table         ← Table data (one Item per line, CORDA format)
    students.map           ← name:ID pairs (loaded first for O(1) lookup)
```

## File Rules
- `.table` file: each line = one Item's data in CORDA format (`key:val;key:val;`)
- `.map` file: each line = `name:ID` pair
- On startup: load `.map` first → then load `.table`
- Every write hits both RAM and disk
- Use `std::ios::app` to avoid overwriting existing files

## Data Format (CORDA)
```
name:Ali;age:21;city:Baghdad;
```
- Delimiter between cells: `;`
- Delimiter between key and value: `:`
- Parsed by C-Text module, not C-Base

## Access Patterns
```cpp
// By ID (O(1) vector index)
students.read(0);

// By name (O(1) via unordered_map)
students.byName("ali");

// Cell value (via C-Text)
corda.get("age", item.data);  // "21"
```

## Table Operations
- `insert(Item)` — auto-assigns ID, updates map, pushes to vector, writes to disk
- `read(int id)` — returns Item by index, returns {-1,"",""} if out of bounds
- `edit(int ID, Item)` — replaces Item at ID, bounds-checked
- `append(Item)` — writes Item data to .table file and name:ID to .map file
- `byName(string)` — lookup via unordered_map → returns Item

## Node Operations
- `addChild(Node*)` — register child node
- `addTable(Table*)` — register table in node
- Path builds from parent chain: `parent.path + "/" + name`

## Code Structure
- `cbase.h` — all class declarations, forward declarations for Node/Table
- `cbase.cpp` — all method implementations
- `filemanager.h` — Append(), Read() disk utilities
- Node.path is public (Table needs access in constructor)
- Item/Cell members are public

## Design Rules
1. Parent always passed by reference in constructor
2. ID auto-assigned by Table on insert (index = vector size)
3. Map loaded into RAM on startup for fast name lookups
4. Bounds-checked with `>=` on all index access
5. No query language — direct API access only
6. C-Base stores, C-Text interprets

## Future Plans
- Large mode: LRU cache with configurable cap (e.g. 1GB max)
- Evict least-recently-used Items when cache full, load from disk on miss
- Listeners/callbacks on data change
- Transactions (batch writes, all-or-nothing)
- Thread safety / locking
