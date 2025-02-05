# Qt Gadgets with Implicit Sharing

A sophisticated C++ implementation demonstrating Qt's implicit sharing pattern through a flexible gadget system. This project showcases advanced C++ features, Qt's meta-object system, and efficient memory management through copy-on-write semantics.

## Overview

This project implements a hierarchy of gadget classes that leverage Qt's implicit sharing capabilities for efficient memory usage and copy-on-write semantics. The implementation provides a robust foundation for creating complex data structures with automatic memory management and optimal performance characteristics.

### Perfect for Complex Data Structures

This implementation is especially useful when you need to:
- Create strongly-typed C++ classes to represent complex JSON structures
- Handle nested data structures with efficient memory usage
- Implement copy-on-write semantics for large data objects
- Maintain clean APIs for complex data manipulation
- Build parent-child relationships with automatic memory management

## Class Hierarchy

- `AbstractGadget`: Base class implementing the core implicit sharing functionality
  - `Gadget`: Simple implementation with a value property
  - `Parent`: Base class for parent objects with lastName property
    - `ParentWithChild`: Demonstrates single child composition
    - `ParentWithChildren`: Demonstrates list-based child composition
  - `Child`: Simple child class used in composition examples

## Key Features

- **Implicit Sharing**: Implements copy-on-write semantics for efficient memory usage
- **Type-Safe Implementation**: Uses modern C++ templates and CRTP pattern
- **Qt Integration**: 
  - Leverages Qt's meta-object system
  - Uses [Q_GADGET](https://doc.qt.io/qt-6/qobject.html#Q_GADGET) for property system integration
  - Provides custom debug output formatting
- **Flexible Architecture**:
  - Base abstract class for common functionality
  - Template-based private implementation pattern
  - Support for complex nested structures
- **JSON Serialization**: Built-in support for JSON serialization and deserialization

## Implementation Details

### AbstractGadget
- Provides the foundation for implicit sharing
- Implements comparison operators
- Handles debug output formatting
- Manages shared data through [QSharedDataPointer](https://doc.qt.io/qt-6/qshareddatapointer.html)
- Implements JSON serialization support

### Derived Classes
- **Gadget**: Simple implementation demonstrating basic value property
- **Parent**: Base class for parent-child relationships with lastName property
- **ParentWithChild**: Shows composition with a single child object
- **ParentWithChildren**: Demonstrates handling lists of child objects
- **Child**: Simple child class with a value property

## Usage Examples

```cpp
// Basic usage with implicit sharing
Gadget gadget;
gadget.setValue(1234);

// Parent-child relationship
ParentWithChild parent;
Child child;
child.setValue(1);
parent.setChild(child);

// Working with lists of children
ParentWithChildren parent;
Child child1, child2;
child1.setValue(1);
child2.setValue(2);
parent.setChildren({child1, child2});

// JSON serialization
const auto jsonString = R"({
    "lastName": "Smith",
    "children": [
        { "value": 2 },
        { "value": 3 }
    ]
})";
ParentWithChildren parent;
parent.fromJsonObject(QJsonDocument::fromJson(jsonString).object());
```

## Build Instructions

### Prerequisites
- Qt 6.5 or later
- C++17 compliant compiler
- CMake 3.14 or later

### Building
```bash
mkdir build
cd build
cmake ..
make
```

## Design Patterns Used

1. **Implicit Sharing Pattern**: For efficient memory usage and copy-on-write semantics
2. **CRTP (Curiously Recurring Template Pattern)**: Used in the private implementation
3. **Composite Pattern**: Demonstrated in parent-child relationships
4. **PIMPL (Private Implementation)**: Used throughout for ABI stability

## Best Practices Demonstrated

- Proper memory management through smart pointers
- Efficient copying through implicit sharing
- Type-safe templates
- Clear separation of concerns
- Consistent error checking
- Property system integration
- JSON serialization support

## Qt Classes Used

This project utilizes several Qt classes:

- [QObject](https://doc.qt.io/qt-6/qobject.html) - Base class for Qt objects
- [QMetaObject](https://doc.qt.io/qt-6/qmetaobject.html) - Contains meta-information about Qt objects
- [QMetaProperty](https://doc.qt.io/qt-6/qmetaproperty.html) - Provides meta-data about a property
- [QSharedData](https://doc.qt.io/qt-6/qshareddata.html) - Base class for implicitly shared objects
- [QSharedDataPointer](https://doc.qt.io/qt-6/qshareddatapointer.html) - Pointer type for implicit sharing
- [QString](https://doc.qt.io/qt-6/qstring.html) - Unicode character string
- [QDebug](https://doc.qt.io/qt-6/qdebug.html) - Debug stream for basic types
- [QJsonObject](https://doc.qt.io/qt-6/qjsonobject.html) - JSON object representation
- [QJsonDocument](https://doc.qt.io/qt-6/qjsondocument.html) - JSON document parser and writer
