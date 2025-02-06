# Qt Gadgets with Implicit Sharing

A sophisticated C++ implementation demonstrating Qt's implicit sharing pattern through a flexible gadget system. This project showcases advanced C++ features, Qt's meta-object system, and efficient memory management through copy-on-write semantics.

## Overview

This project implements a hierarchy of gadget classes that leverage Qt's implicit sharing capabilities for efficient memory usage and copy-on-write semantics. The implementation includes a comprehensive test suite demonstrating various use cases and supported data types.

### Perfect for Complex Data Structures

This implementation is especially useful when you need to:
- Create strongly-typed C++ classes with Qt's property system
- Handle nested data structures with efficient memory usage
- Implement copy-on-write semantics for large data objects
- Support a wide range of Qt data types including lists and enums
- Build parent-child relationships with automatic memory management

## Class Hierarchy

- `AbstractGadget`: Base class implementing the core implicit sharing functionality
  - `Gadget`: Simple implementation with a value property
  - `Child`: Reusable child class for composition examples
  - `Parent`: Base class for parent objects with lastName property
    - `ParentWithChild`: Demonstrates single child composition
    - `ParentWithChildren`: Demonstrates list-based child composition
  - `Types`: Comprehensive demonstration of supported property types

## Key Features

- **Implicit Sharing**: Implements copy-on-write semantics for efficient memory usage
- **Type-Safe Implementation**: Uses modern C++ templates and CRTP pattern
- **Qt Integration**: 
  - Leverages Qt's meta-object system
  - Uses [Q_GADGET](https://doc.qt.io/qt-6/qobject.html#Q_GADGET) for property system integration
  - Provides custom debug output formatting
- **Comprehensive Type Support**:
  - Basic types (bool, int)
  - Qt types (QString, QByteArray, QJsonObject)
  - List types (QList&lt;T&gt;)
  - Enum types with Q_ENUM support
  - Nested gadget types
- **JSON Serialization**: Built-in support for JSON serialization and deserialization

## Implementation Details

### AbstractGadget
- Provides the foundation for implicit sharing
- Implements comparison operators
- Handles debug output formatting
- Manages shared data through [QSharedDataPointer](https://doc.qt.io/qt-6/qshareddatapointer.html)
- Implements JSON serialization support

### Supported Property Types
The `Types` class demonstrates all supported property types:
- Boolean values and lists
- Integer values and lists
- QByteArray and QByteArrayList
- QString and QStringList
- QJsonObject
- Custom enums and enum lists
- Nested gadget objects and lists

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
    "boolean": true,
    "integers": [1, 2, 3],
    "string": "test",
    "enumeration": "debug",
    "child": { "value": 1 }
})";
Types types;
types.fromJsonObject(QJsonDocument::fromJson(jsonString).object());
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
- Type-safe templates and property system
- Comprehensive unit testing
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
- [QtTest](https://doc.qt.io/qt-6/qtest.html) - Unit testing framework
