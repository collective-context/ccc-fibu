# CLAUDE.md - FiCore Developer Guide for AI Assistants

## Project Overview

**FiCore - Open Source Financial Core** is a modernization of a legacy C/MSDOS FIBU (Finanzbuchhaltung - Accounting) system. It's built on decades of production experience and is being transformed from a DOS-based Btrieve database system to a modern, open-source web-first architecture.

**Vision**: Financial freedom and transparency through privacy-first, self-hosted accounting software. Part of the CCC ecosystem (HUB.CY, recode.at, osCASH.me).

**Status**:
- Legacy code fully analyzed and documented
- UTF-8 conversion complete (440+ MASK/FORM files, 28 .ORG files)
- Btrieve → SQL exporter (THE KEY TOOL) production-ready
- Developer Handbook in progress
- Modern web UI still in planning phase

---

## Table of Contents

1. [Overall Project Structure](#1-overall-project-structure)
2. [Programming Languages & Frameworks](#2-programming-languages--frameworks)
3. [Key Source Directories](#3-key-source-directories--their-purposes)
4. [Configuration Files](#4-configuration-files--their-roles)
5. [Documentation Structure](#5-documentation-structure)
6. [Build & Deployment](#6-build--deployment-setup)
7. [Testing Infrastructure](#7-testing-infrastructure)
8. [Domain-Specific Patterns](#8-domain-specific-patterns--architectural-decisions)
9. [Code Conventions](#9-code-conventions--patterns)
10. [Key Workflows](#10-key-workflows--development-processes)
11. [Important Files Quick Reference](#11-important-files-quick-reference)
12. [Technology Debt & Migration Path](#12-technology-debt--migration-path)
13. [Repository & Git Workflow](#13-repository--git-workflow)
14. [Task-Specific Guides](#14-for-specific-tasks)
15. [Quick Troubleshooting](#15-quick-troubleshooting)
16. [Final Notes for AI Assistants](#16-final-notes-for-ai-assistants)

---

## 1. Overall Project Structure

```
ccc-fibu/
├── docs/                           # Comprehensive documentation
│   ├── book-01/                    # Developer Handbook (for AI assistants & new devs)
│   │   ├── README.md               # Handbook guide
│   │   └── chapter-01-case-tool-fundamentals.md  # Core CASE tool concepts
│   ├── BTRIEVE_*.md                # 5 detailed technical docs
│   ├── CASE_TOOL_MASK_FORM_SYSTEM.md  # CASE tool deep dive (55 KB)
│   └── README_BTRIEVE.md           # Btrieve overview
│
├── euro_UTF8/                      # ACTIVE DEVELOPMENT COPY (UTF-8 encoded)
│   ├── C/                          # 84 C++ files (~61K lines) - Main business logic
│   ├── INCLUDE/                    # 3 header files - Type definitions & APIs
│   ├── case/                       # SAA CASE tool (219 C++ files for code generation)
│   │   ├── ISAM/                   # Btrieve ISAM interface (28 files)
│   │   ├── SAA/                    # CASE compiler (140+ files)
│   │   └── ...
│   ├── MASK/                       # 260 screen mask definitions (UI + Schema)
│   ├── FORM/                       # 199 form/print layouts
│   ├── DAT/D01/2024/               # Test data (Btrieve .btr files)
│   ├── BAT/                        # Build/utility batch files
│   └── ...
│
├── euro_DOS (read-only)/           # ORIGINAL LEGACY CODE (archived, CP850)
│   └── ... (same structure as euro_UTF8)
│
├── euro/                           # Legacy MSDOS system (deprecated)
│
├── tools/                          # Development utilities
│   ├── btrieve_to_sql_exporter.py  # THE KEY TOOL: Btrieve → SQLite/PostgreSQL
│   ├── btrieve_parser.py           # Btrieve file parser
│   ├── convert_*.py                # UTF-8 encoding conversion tools
│   ├── modernize_headers.py        # Header rebranding tool
│   └── *.sh                        # Bash equivalents
│
├── .logs/                          # Session logs and analysis notes
│
├── README.md                       # Main project documentation (19 KB)
├── REBRANDING.md                   # Rebranding history (5 KB)
├── HEADER_MODERNIZATION.md         # Header modernization details (6 KB)
└── CLAUDE.md                       # This file - AI assistant guide
```

---

## 2. Programming Languages & Frameworks

### Primary Languages

| Language | Usage | Files | Lines | Purpose |
|----------|-------|-------|-------|---------|
| **C/C++** | Legacy core system | 84 cpp + headers | ~61,500 | FIBU business logic, UI, Btrieve access |
| **Python** | Modern tooling | 8 files | ~2,000 | Data migration, code generation, utilities |
| **Bash** | Build/deployment | 6 scripts | ~400 | File conversion, batch processing |

### Legacy Technology Stack

- **CASE Tool**: SAA (Screen Application Architecture) - automatic code generation from MASK definitions
- **Database**: Btrieve 5.x/6.x (ISAM database engine for DOS)
- **Encoding**: CP850 (DOS German) → UTF-8 (modern)
- **Build**: DOS/Windows batch files (legacy build system)
- **UI Framework**: Character-based screens with MASK/FORM system

### Modern Technology Stack (Target)

- **Backend**: FastAPI (Python)
- **Frontend**: React/Vue (TypeScript)
- **Database**: SQLite (development) / PostgreSQL (production)
- **API**: REST (with GraphQL planned)
- **Deployment**: Docker containers, cloud-native

---

## 3. Key Source Directories & Their Purposes

### `euro_UTF8/C/` - Main Business Logic (84 files, ~61.5K lines)

**Purpose**: Core FIBU application logic and UI handlers

**File Organization**:
- **fi*.cpp** (30+ files): Financial (FIBU) modules
  - `fi1100.cpp` - Customer master data
  - `fi1210.cpp` - Vendor master data
  - `fi1310.cpp` - Chart of accounts
  - `fi2100.cpp` - Booking entry
  - `fi3100*.cpp` - Journal/booking entries
  - `fi4100*.cpp` - Reports and statements

- **eu*.cpp** (9 files): Euro/Currency conversion modules
  - `eu1100.cpp` - EUR currency entry
  - `eu2100.cpp` - EUR booking processing
  - `eu4100.cpp` - EUR reports

- **st*.cpp** (20+ files): Statistics and analysis modules
  - `st1100.cpp` - Statistical data entry
  - `st4100.cpp` - Statistical reports

- **sy*.cpp** (4 files): System administration
  - `sy1100.cpp` - System parameters
  - `sy4100.cpp` - System configuration

- **pb*.cpp** (3 files): Print/report modules
  - `pb1100.cpp` - Print dialogs
  - `pb4100.cpp` - Batch printing

**Architecture Pattern**:
```c
Each module (e.g., fi1100.cpp):
1. Includes app_*.h headers (type definitions, APIs)
2. Includes ba.h and ba_proto.h (base application framework)
3. Defines STATIC functions (private) and GLOBAL functions (exported)
4. Implements functions following naming pattern: ModulePrefix_FunctionName()
5. Uses field access macros: pt(), ptD(), ptW() for memory-safe field access
```

**Numeric Code Convention**:
- **1xxx** - Master data entry/maintenance
- **2xxx** - Transaction entry/processing
- **3xxx** - Journal/permanent records
- **4xxx** - Reports/output
- **5xxx** - Utilities/tools

### `euro_UTF8/INCLUDE/` - Header Files & Type Definitions

**Purpose**: Foundational types, macros, and API definitions

- **app_type.h**: Core data types with naming conventions
  - Typedef patterns: `CHAR`, `SWORD`, `DOUBLE`, etc.
  - Pointer patterns: `PSTR` (pointer to string), `PSWORD` (pointer to SWORD)
  - Type prefixes for variable names (hungarian notation)

- **app_btr.h**: Btrieve database interface
  - BTRV() low-level interrupt function
  - Db_*() high-level functions (GetEq, Insert, Update, Delete, etc.)
  - File operation codes (B_OPEN, B_INS, B_GET_EQ, etc.)
  - Record structure templates (FSPEC, KSPEC)

- **Other headers** (app_bas.h, app_dlg.h, app_msk.h, etc.): UI components, dialogs, masks

### `euro_UTF8/case/` - Code Generation System (219 files)

**Purpose**: SAA (Screen Application Architecture) CASE tool for automatic code generation

**Subdirectories**:

1. **ISAM/** - Btrieve interface implementation
   - `btrv.cpp` - Low-level BTRV() wrapper
   - `db_*.cpp` (12+ files) - High-level Db_*() functions
   - `*.ORG` - Documentation files (28 files, UTF-8 converted)

2. **SAA/** - CASE compiler and code generator
   - Generates C code from MASK definitions
   - Parses MASK files and extracts field definitions
   - Generates Btrieve record access code automatically

3. **DOS/** - DOS-specific utilities
   - Interrupt handling, DOS API wrappers

### `euro_UTF8/MASK/` - Screen Definitions (260 files)

**Purpose**: Declarative UI + database schema definitions

**Key insight**: MASK files simultaneously define:
1. **UI Layout** - Screen appearance with box-drawing characters
2. **Data Schema** - Field names, types, lengths (via placeholder symbols)
3. **Database Mapping** - Directives (`&DATA`, `&DATEI`) linking fields to Btrieve records

**Organization by module**:
- **FI/** - Financial/FIBU masks (~50 files)
- **ST/** - Statistics masks (~30 files)
- **SY/** - System masks (~20 files)
- **FIBU/** - Additional FIBU masks
- **SYSTEM/** - System-level masks

**File Format Example**:
```
[ASCII-ART LAYOUT with placeholders]
  ^       = 1 char field
  ^^      = 2 char field
  ^^^^^^^^ = 8 char field
  #####    = 5-digit number
  ###.##   = Amount with decimal
  @        = Color/attribute marker

[METADATA SECTION]
>>Mask:   FIM131
>>Prog.:  FI1100.C
>>Update: 25.07.91

[DIRECTIVES SECTION]
&TITEL    = "Mask title"
&DATEI    = BTRIEVE_FILE_REFERENCES
&DATA     = FIELD_MAPPINGS
&CHOICE   = SELECTION_LISTS
&HELPTEXT = HELP_REFERENCES
```

### `euro_UTF8/FORM/` - Print Layouts (199 files)

**Purpose**: Form/report layout definitions for printing

Similar structure to MASK but focused on print output formatting.

### `euro_UTF8/DAT/D01/2024/` - Test Data (Btrieve Files)

**Data Files**:
- **FI1110.btr** (18 KB) - Customer master data (Kunden)
- **FI1115.btr** (26 KB) - Customer open items (Offene Posten Kunden)
- **FI1210.btr** (9 KB) - Vendor master data (Lieferanten)
- **FI1215.btr** (23 KB) - Vendor open items
- **FI1310.btr** (29 KB) - Chart of accounts (Sachkonten)
- **FI2100.btr** (151 KB) - Entry register (Erfassungsdatei) - editable
- **FI2100a.btr** (290 KB) - Journal/control file (Kontrolldatei) - posted
- **FI3100.btr** (249 KB) - Full journal (Vollständiges Journal) - final
- **SYP_50.BTR** (10 KB) - System parameters

**Record Structure**: Variable-length records (typically 100-430 bytes)

---

## 4. Configuration Files & Their Roles

### `.gitignore`
Ignores compiled binaries, IDE files, build artifacts, and sensitive data (`.key`, `.pem` files).

### `euro/4win.ini`
Legacy configuration file with client/database settings.

### Batch Files (`euro_UTF8/BAT/`)
- ***.B files**: Compiled batch scripts (binary format)
- **BUILD.BAT**: Main build script
- **UPDATE.BAT**: Update script
- Legacy DOS build system files

---

## 5. Documentation Structure

### Primary Documentation

1. **README.md** (460 lines)
   - Project vision and history
   - Btrieve format details
   - C code structure overview
   - Key files and records
   - Development tools and commands
   - Technology stack

2. **REBRANDING.md**
   - Phase 1: Brand updates (320+ files)
   - Phase 2: Header modernization (59 files)
   - Phase 2.1: Author line simplification (49 files)
   - Total: 428 files updated with new FiCore identity

3. **HEADER_MODERNIZATION.md**
   - Header style changes (4win.com → CCC/FiCore)
   - Old vs. new patterns
   - Tools used for automation

### Technical Documentation

4. **docs/BTRIEVE_RECORD_STRUCTURES.md**
   - Record definitions for all .BTR files
   - Field offsets, types, lengths
   - Index definitions
   - C struct definitions

5. **docs/BTRIEVE_ISAM_INTERFACE.md** (39 KB)
   - Low-level BTRV() interface
   - High-level Db_*() functions
   - Multi-user locking strategies
   - Transactions and error handling
   - Best practices

6. **docs/CASE_TOOL_MASK_FORM_SYSTEM.md** (55 KB)
   - MASK file format and syntax
   - Placeholder symbols and field types
   - Directive reference (&DATEI, &DATA, &CHOICE, etc.)
   - Automatic code generation
   - Multi-file integration patterns

7. **docs/BTRIEVE_FILE_HIERARCHY.md**
   - Data organization by year, client, module
   - File naming conventions
   - Relationships between files

8. **docs/BTRIEVE_TO_SQL_EXPORT.md** (41 KB)
   - THE KEY TOOL documentation
   - Usage examples
   - Schema generation
   - Validation rules
   - SOLL=HABEN accounting validation

### Developer Handbook (NEW!)

9. **docs/book-01/README.md**
   - Guide for new developers and AI agents
   - Chapter overview and status
   - Quick start paths
   - Learning progression

10. **docs/book-01/chapter-01-case-tool-fundamentals.md**
    - Detailed CASE tool explanation
    - Step-by-step structure derivation
    - Practical examples (FI1310 Sachkonten)
    - Copy-paste code examples

---

## 6. Build & Deployment Setup

### Legacy Build System

**Current State**: DOS batch files (*.B, *.BAT)
- `BUILD.BAT` - Main build script
- `UPDATE.BAT` - Update/incremental build
- Module-specific batch files (FIMASK.BAT, STMASK.BAT, etc.)

**Build Process**:
1. CASE compiler (SAA) converts MASK → C code
2. C/C++ compiler compiles source files
3. Linker creates EXE files
4. Batch scripts orchestrate the process

### Modern Deployment Target

**Planned Architecture**:
```
Source (Btrieve) → Python Parser → SQLite/PostgreSQL
                ↓
            FastAPI Backend
                ↓
         React/Vue Frontend
```

### Key Tool: btrieve_to_sql_exporter.py

**Purpose**: THE KEY TOOL - One-way export from Btrieve to SQL

**Features**:
- CP850 → UTF-8 encoding conversion
- Automatic schema generation
- Primary key and index creation
- SOLL=HABEN validation for accounting entries
- Idempotent (can run repeatedly)
- Dry-run mode for testing
- Performance: ~950 records in ~1 second (SQLite)

**Usage**:
```bash
# SQLite export
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu.db

# PostgreSQL export
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target "postgresql://user:pass@localhost/fibu" \
    --db-type postgresql

# Dry-run (preview only)
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu.db \
    --dry-run
```

---

## 7. Testing Infrastructure

### Current State

**Minimal formal testing** - Legacy system predates modern testing practices

**Test Files Found**:
- `euro_UTF8/C/test.cpp` - Basic test code
- `euro_UTF8/case/SAA/test/` - CASE compiler tests

### Testing Strategy (Recommended)

**For Modern Development**:
1. **Unit Tests**: Python pytest for tools (btrieve_parser, exporter)
2. **Integration Tests**: Btrieve → SQLite → REST API validation
3. **Data Validation**: SOLL=HABEN checks, record count validation
4. **Encoding Tests**: CP850 → UTF-8 round-trip validation

### Data Validation Built-in

The btrieve_to_sql_exporter includes:
- SOLL=HABEN (debit=credit) validation for journal entries
- Record structure validation
- Field type validation
- Encoding detection and conversion

---

## 8. Domain-Specific Patterns & Architectural Decisions

### Pattern 1: MASK = UI + Schema

**Key Innovation**: MASK files are NOT just UI descriptions; they simultaneously define:
1. **Visual Layout** - Character positions, boxes, labels
2. **Data Schema** - Field names, types, lengths via placeholder counts
3. **Database Binding** - `&DATA` directives linking to Btrieve records

**Example**: `FIM131` (Account Balances)
```
Konto: S^^^^^^^^            # 8-char field (S = type indicator, ^ = char)
Saldo: ###.###.##ø,øø       # Amount field (5 digits + 2 decimals)

&DATA (+-) = 0, KONTONR     # Maps field to record 0, field KONTONR
&DATA (+-) = 0, SALDO       # Maps field to record 0, field SALDO
```

**Placeholder Mapping**:
- `^` = 1 char → CHAR(1)
- `^^^^^^^^` = 8 chars → CHAR(8)
- `#####` = 5 digits → NUMBER(5)
- `###.##` = Amount with 2 decimals → DECIMAL(5,2)

**Modern Evolution**: MASK → JSON Schema → React Components (planned)

### Pattern 2: Multi-File Integration

**Single MASK can reference 5+ Btrieve files**:
- `REC_F` (0) - Main record file
- `REF_F` (1) - Reference/lookup files
- `DRU_F` (2) - Print/report files
- `WORK_F` (3) - Working/temporary files
- `PARA_F` (6) - Parameter files

**Example**: Booking entry (FIM210) integrates:
- FI2100.btr - Entry register
- FI1110.btr - Customer lookup
- FI1210.btr - Vendor lookup
- FI1310.btr - Account lookup
- FI1500.btr - Cost center lookup

**Directives**:
```
&DATEI (0) = MASK, D.FI2100      # Primary file
&REF_DATEI (1) = MASK, D.FI1110  # Reference file for F1 lookups
```

### Pattern 3: Field Access via Macros

**Memory-safe field access** without explicit struct offsets:

```c
// Unsafe approach (error-prone)
char* ptr = record_buffer;
int value = *(int*)(ptr + 12);  // Magic number!

// FiCore pattern - Safe via macros
pt("FELDNAME")      // Pointer to field (char*)
ptD("FELDNAME")     // Double field access
ptW("FELDNAME")     // Word field access (SWORD)
```

**Implementation**: Macro looks up field definition from record structure, calculates offset safely.

### Pattern 4: Accounting Business Rules

**Critical Invariants**:
1. **SOLL = HABEN** - Debit must always equal Credit
2. **Three-Stage Processing**:
   - FI2100.btr - Entry (editable, can be corrected)
   - FI2100a.btr - Control/Journal (posted, immutable except via reversal)
   - FI3100.btr - Full Journal (complete audit trail)
3. **Reversal Only** - No deletion after posting; use Storno (reversal) entries
4. **Audit Trail** - All changes logged with timestamp and user

**Shadow Balances** (`Schattensaldo`):
- Real-time balance calculation BEFORE posting
- Allows "what-if" scenarios
- Updated automatically during entry

### Pattern 5: Modular Code Organization

**Module Naming Convention**:
- **Module Prefix** identifies functionality: `fi` (FIBU), `eu` (EURO), `st` (stats), `sy` (system)
- **Numeric Code** identifies operation: `1100` (master), `2100` (entry), `3100` (journal), `4100` (reports)
- **Suffix** indicates variant: `a`, `b`, `c` for multiple implementations

**Example**:
- `fi1100.cpp` - FIBU master entry (customers)
- `fi2100.cpp` - FIBU booking entry (new entries)
- `fi3100.cpp` - FIBU journal (posted entries)
- `fi4100.cpp` - FIBU reports
- `fi3100a.cpp` - Alternative journal implementation

**Architectural Benefit**: Clear separation of concerns, easy to parallelize development

### Pattern 6: Global Variable Usage

**Note**: Legacy system uses extensive global variables via `GLOBAL` macro.

```c
GLOBAL SWORD  wSteuerKz_g;      // Control code (g = global, sw = signed word)
IMPORT SWORD  wMaxiMask_g;      // Imported from another module
STATIC SWORD  wFormular_m;      // Module-static variable (m = module)
```

**Convention**:
- `_g` suffix = global variable
- `_m` suffix = module-static variable
- Prefix = type indicator (HUNGARIAN NOTATION)

**Modernization Note**: Replace with dependency injection and clean architecture in new code.

---

## 9. Code Conventions & Patterns

### Naming Conventions

**Variables** (Hungarian notation):
```c
CHAR  cMyChar;              // c = char
PSTR  pstrName;             // pstr = pointer to string
SWORD swValue;              // sw = signed word
PSWORD pswCounter;          // psw = pointer to signed word
DOUBLE dAmount;             // d = double
PDOUBLE pdValue;            // pd = pointer to double
SWORD (*pfnHandler)();      // pfn = pointer to function
```

**Functions**:
- `Module_Prefix_ActionName()` pattern
- Example: `Fi210_Reference_Key_Handler()`, `Update_Salden()`
- Lowercase for utility functions: `i_Kto_Empty()`, `i_SynCheck()`

**Macros**:
- `UPPERCASE_WITH_UNDERSCORES`
- Example: `B_OPEN`, `B_INS`, `MAX_RECORD_SIZE`

**File Organization**:
```c
// 1. Includes section
#include <string.h>
#include <app_tool.h>
#include "..\c\ba.h"

// 2. Global imports
IMPORT SWORD wMaxiMask_g;

// 3. Function prototypes
GLOBAL SWORD Fi220_Application_Set(SWORD, SWORD);

// 4. Module-level declarations
STATIC SWORD wFormular_m;
STATIC DOUBLE dZeitWert_m;

// 5. Main function(s)
VOID main(SWORD argc, PSTR argv[]) { ... }

// 6. Helper functions
STATIC VOID i_SynDef(...) { ... }
```

### Coding Style

**Indentation**: 2-4 spaces (mixed in legacy code)

**Braces**:
```c
// Style 1 (most common in codebase)
if(condition)
  {
  statement1;
  statement2;
  }

// Style 2 (in some files)
if (condition) {
    statement;
}
```

**Comments**:
```c
// Single line comments preferred in newer code
/* Multi-line comments
   for blocks */

// Special markers:
//!  IMPORTANT NOTE
//TODO Future work
//BUG Known issue
```

### Error Handling

**Btrieve Status Codes**:
- `0` - Success
- `1` - I/O error
- `2` - Key value not found
- `3` - Invalid key number
- etc. (see BTRIEVE_ISAM_INTERFACE.md)

**Pattern**:
```c
SWORD wStatus = Db_GetEq(...);
if (wStatus != 0) {
    // Handle error
    return wStatus;
}
```

**Database Transactions**:
```c
BTRV(B_BEGIN_TRAN, ...);
if (error_condition) {
    BTRV(B_ABORT_TRAN, ...);
    return ERROR;
}
BTRV(B_END_TRAN, ...);
```

---

## 10. Key Workflows & Development Processes

### Workflow 1: Understanding Existing Functionality

**For AI assistants starting new work**:

1. **Identify the MASK file** for the feature (e.g., `euro_UTF8/MASK/FI/FIM131`)

2. **Analyze the MASK**:
   - Count placeholder symbols to determine field lengths
   - Identify `&DATA` directives linking to Btrieve records
   - Note `&DATEI` directives showing which files are used

3. **Find the C implementation**:
   - Mask metadata shows C file (e.g., `>>Prog.: FI1100.C`)
   - Search for function implementing the mask

4. **Read the Btrieve structure documentation**:
   - `docs/BTRIEVE_RECORD_STRUCTURES.md` for field definitions
   - Check record offsets and types

5. **Trace the data flow**:
   - Entry → Validation → Database insert/update → Display

### Workflow 2: Exporting Data for Modern Development

**When migrating features to web stack**:

1. **Export Btrieve data to SQL**:
```bash
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu.db
```

2. **Validate exported data**:
   - Check record counts match
   - Verify SOLL=HABEN balance
   - Spot-check key values

3. **Design SQL schema** based on exported tables

4. **Implement REST API** for that entity

5. **Create React component** to display data

6. **Test against legacy system** data

### Workflow 3: Adding New Features

**To add new accounting feature**:

1. **Design MASK file** (`euro_UTF8/MASK/FI/FIMyFeature`)
   - Layout, fields, data directives

2. **Run CASE compiler** (or manual MASK parser):
   - Generates field definitions

3. **Design Btrieve record structure** or use existing files

4. **Implement C code** in `euro_UTF8/C/fi*.cpp`:
   - UI handling (display, input)
   - Validation logic
   - Database operations

5. **Test with legacy system**

6. **Export to SQL** when ready

7. **Implement REST API** endpoint

8. **Create modern React UI**

### Workflow 4: Code Modernization

**Recommended for each existing module**:

1. **Analyze** the C code → understand business logic
2. **Write tests** against exported SQL data
3. **Implement** equivalent logic in Python/FastAPI
4. **Create React UI** from MASK definition
5. **Validate** outputs match legacy system
6. **Migrate** users to new interface
7. **Archive** C code for reference

---

## 11. Important Files Quick Reference

### Start Here for New Sessions

| What you need | File to read | Time |
|---|---|---|
| Project overview | README.md | 10 min |
| AI assistant guide | CLAUDE.md (this file) | 20 min |
| CASE tool basics | docs/book-01/chapter-01-case-tool-fundamentals.md | 20 min |
| Understanding MASK files | docs/CASE_TOOL_MASK_FORM_SYSTEM.md | 30 min |
| Btrieve details | docs/README_BTRIEVE.md | 10 min |
| Data export | docs/BTRIEVE_TO_SQL_EXPORT.md | 15 min |
| Record structures | docs/BTRIEVE_RECORD_STRUCTURES.md | 20 min |

### Key Examples

| Task | File | Example |
|------|------|---------|
| View MASK file | `euro_UTF8/MASK/FI/FIM131` | Account balances definition |
| View C implementation | `euro_UTF8/C/fi1100.cpp` | Customer master data logic |
| View Btrieve struct | `docs/BTRIEVE_RECORD_STRUCTURES.md` | FI1310 record definition |
| Run exporter | `tools/btrieve_to_sql_exporter.py` | Btrieve → SQLite export |
| Parse Btrieve | `tools/btrieve_parser.py` | Low-level file parsing |

---

## 12. Technology Debt & Migration Path

### Known Limitations

1. **Character-Based UI**: Not suitable for modern web
2. **Global Variables**: Hard to test and parallelize
3. **Legacy Batch Build**: Difficult to automate
4. **CP850 Encoding**: Limited internationalization
5. **Btrieve Dependency**: Not cloud-friendly
6. **DOS Platform**: No longer actively supported

### Migration Strategy

**Phase 1 (DONE)**:
- ✅ UTF-8 conversion
- ✅ Code analysis
- ✅ Comprehensive documentation
- ✅ Developer handbook started

**Phase 2 (IN PROGRESS)**:
- ✅ Btrieve → SQL export tool (production-ready)
- 🔄 REST API design
- 🔄 Schema finalization

**Phase 3 (PLANNED)**:
- ⏳ React UI prototypes
- ⏳ FastAPI backend implementation
- ⏳ Authentication/authorization

**Phase 4 (PLANNED)**:
- ⏳ Feature parity with legacy system
- ⏳ User acceptance testing
- ⏳ Performance optimization

**Phase 5 (PLANNED)**:
- ⏳ Production deployment
- ⏳ Multi-tenant support
- ⏳ Cloud hosting

**Phase 6 (PLANNED)**:
- ⏳ Sunset legacy system
- ⏳ Open source release
- ⏳ Community building

---

## 13. Repository & Git Workflow

### Branch Information

**Current Development Branch**: `claude/claude-md-mi1jh2im7vibgnnf-01KZU2G3dyhuqzUmFABhfTLb`

**Remote**: `collective-context/ccc-fibu` (private repository)

### Recent Commits

```
955f618 🎓 Add Developer Handbook - Chapter 1: CASE Tool Fundamentals
80a70f5 FiCore - Open Source Financial Core
7e40457 Weitere Analysen erstellt
5d55832 Weitere Analysen erstellt
08daaa8 Weitere Analysen erstellt
```

### Commit Conventions

**Format**: `[emoji] Type: Description` or `[type](scope): description`

Examples from history:
- `🎓 Add Developer Handbook` - Documentation updates
- `FiCore - Open Source Financial Core` - Major rebranding
- `feat: Complete UTF-8 conversion` - Feature completion
- `fix: Correct encoding issues` - Bug fixes

### Git Best Practices

1. **Always develop on the designated branch** (starts with `claude/`)
2. **Commit frequently** with clear messages
3. **Push with `-u origin <branch-name>`** for new branches
4. **Never force push** without explicit permission
5. **Test before committing** when possible

---

## 14. For Specific Tasks

### Task: Understand how FI3100 (Journal) works

1. **Read MASK**: `euro_UTF8/MASK/FI/FIM310` (or search for journal in MASK/)
2. **Understand structure**: `docs/BTRIEVE_RECORD_STRUCTURES.md` → FI3100.btr section
3. **Read code**: `euro_UTF8/C/fi3100*.cpp` (likely split across 3-4 files)
4. **Export data**: `python3 tools/btrieve_to_sql_exporter.py`
5. **Analyze**: SQL table `fi3100_journal` in fibu.db

### Task: Add new report

1. **Design**: Create MASK file in `euro_UTF8/MASK/FI/`
2. **Define**: Record structure in `docs/BTRIEVE_RECORD_STRUCTURES.md`
3. **Implement**: C code in `euro_UTF8/C/fi4*.cpp` (reports use 4xxx series)
4. **Test**: Legacy system validation
5. **Export**: Btrieve → SQL
6. **Modernize**: REST API + React component

### Task: Fix encoding issue

1. **Tool**: `python3 tools/convert_*.py --help`
2. **Apply**: `python3 tools/convert_org_to_utf8.py euro_UTF8/case/ISAM/`
3. **Verify**: `file /path/to/file` → should show UTF-8
4. **Commit**: Include encoding fix in commit message

### Task: Export all data to SQLite

```bash
# Full export with validation
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu_complete.db \
    --verbose

# Verify export
sqlite3 fibu_complete.db "SELECT name FROM sqlite_master WHERE type='table';"
sqlite3 fibu_complete.db "SELECT COUNT(*) FROM fi3100_journal;"
```

---

## 15. Quick Troubleshooting

### "Which file contains this function?"

```bash
grep -r "function_name" euro_UTF8/C/
grep -r "function_name" euro_UTF8/INCLUDE/
```

### "How is FI1310 structured?"

```bash
# 1. Find MASK definition
find euro_UTF8/MASK -name "*1310*"

# 2. View the MASK
cat euro_UTF8/MASK/FI/[filename]

# 3. Count placeholder symbols for field lengths
# 4. Find documentation
grep -A 50 "FI1310" docs/BTRIEVE_RECORD_STRUCTURES.md
```

### "How to export specific Btrieve file?"

```bash
# Use the parser for single file
python3 tools/btrieve_parser.py euro_UTF8/DAT/D01/2024/FI3100.btr

# Or use exporter with specific file selection
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target test.db \
    --dry-run          # Preview first!
```

### "Which C code handles mask FIM123?"

```bash
# MASK metadata shows the C file
grep ">>Prog.:" euro_UTF8/MASK/FI/FIM123

# Or search for mask name in C code
grep -r "FIM123" euro_UTF8/C/*.cpp
```

### "What does this Btrieve status code mean?"

See `docs/BTRIEVE_ISAM_INTERFACE.md` → Error Codes section

Common codes:
- `0` = Success
- `2` = Key not found
- `9` = Duplicate key
- `84` = Record locked

### "How to understand German variable names?"

Common German accounting terms:
- **Konto** = Account
- **Saldo** = Balance
- **Buchung** = Booking/Entry
- **Beleg** = Document/Voucher
- **Soll** = Debit
- **Haben** = Credit
- **Sachkonto** = General Ledger Account
- **Debitor** = Customer/Debtor
- **Kreditor** = Vendor/Creditor
- **OP** = Offene Posten (Open Items)
- **MwSt** = Mehrwertsteuer (VAT)

---

## 16. Final Notes for AI Assistants

### This is a REAL system with REAL history

- **Not a tutorial project** - Originally built in 1989-1999 for professional use
- **Decades of production experience** - Features are well-tested and battle-hardened
- **Character-based UI** - Reflects 1990s technology but design patterns are elegant
- **Still in use** - Legacy system is actively used; changes must be backward-compatible

### Key Principles

1. **Preserve Data Integrity** - Financial data is critical; validation is essential
2. **Respect Legacy Design** - Old design patterns often have good reasons (efficiency, reliability)
3. **Document Discoveries** - Add to `docs/` when you learn something new
4. **Test Thoroughly** - Especially for accounting logic (SOLL=HABEN!)
5. **Migrate Carefully** - Run new code in parallel with legacy system first
6. **Never Assume** - Always verify against documentation or source code

### What Makes This System Special

1. **CASE Tool Architecture** - One of the earliest low-code platforms
2. **Declarative Programming** - MASK files define both UI and schema
3. **Code Generation** - Automatic C code from high-level definitions
4. **Separation of Concerns** - Data, UI, and logic cleanly separated
5. **Financial Rigor** - Double-entry bookkeeping with audit trails
6. **Multi-User Design** - Record locking and transactions from day one

### Common Pitfalls to Avoid

1. **Don't ignore SOLL=HABEN validation** - This is fundamental to accounting
2. **Don't assume ASCII encoding** - Always check for CP850 and convert to UTF-8
3. **Don't modify journal entries** - Use reversal/Storno instead
4. **Don't skip the MASK files** - They contain critical schema information
5. **Don't break backward compatibility** - Legacy system must keep working
6. **Don't guess at field types** - Count the placeholders in MASK files

### When to Ask for Clarification

- Financial domain logic you're unsure about (ask user or check docs)
- German terms you don't understand (see troubleshooting section)
- Encoding issues (could be CP850, UTF-8, or mixed)
- Complex transaction logic (may need domain expert)
- Migration strategy decisions (coordinate with project owner)

### Resources for Learning More

1. **Start with**: `docs/book-01/chapter-01-case-tool-fundamentals.md`
2. **Then read**: `docs/CASE_TOOL_MASK_FORM_SYSTEM.md`
3. **Understand data**: `docs/BTRIEVE_RECORD_STRUCTURES.md`
4. **Practice with**: `tools/btrieve_to_sql_exporter.py`
5. **Explore code**: Pick a simple module like `fi1100.cpp` (customer master)

### What's Next for FiCore?

**Immediate priorities**:
- [ ] Complete Developer Handbook (chapters 2-8)
- [ ] Build REST API for core entities
- [ ] Create React prototypes for key features
- [ ] Implement automated data export pipeline
- [ ] Set up CI/CD for modern codebase

**Medium term**:
- [ ] Migrate first test users to new system
- [ ] Performance benchmarking
- [ ] Security audit
- [ ] Multi-tenant support
- [ ] Cloud deployment

**Long term**:
- [ ] Open source release
- [ ] Community building
- [ ] Integration with osCASH.me
- [ ] Mobile apps
- [ ] API ecosystem

---

## Contributing to CLAUDE.md

**This file should evolve** as new patterns, workflows, and insights are discovered.

**How to update**:
1. Add new sections as you discover patterns
2. Update task-specific guides when you solve a problem
3. Add troubleshooting entries for common issues
4. Include examples and code snippets
5. Link to new documentation files
6. Keep the table of contents updated

**Commit message format**:
```
docs: Update CLAUDE.md - [brief description]

- Added section on [topic]
- Updated workflow for [task]
- Fixed typo in [section]
```

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2025-11-16 | Initial comprehensive guide created |

---

**Last Updated**: 2025-11-16
**Status**: Production Guide for AI Assistants
**Audience**: Developers, AI Assistants, Technical Architects
**Project**: FiCore - Open Source Financial Core

*"Code with consciousness. Build with love. Share with freedom."* 🙏

**For financial freedom and transparency. For our children.**
