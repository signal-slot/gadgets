# Gadgets Library

A modern C++ library for creating type-safe, serializable data objects using Qt's meta-object system.

## Features

- Type-safe data objects with property support
- Automatic JSON serialization/deserialization for a wide range of types
- Memory-safe implementation using Qt's implicit sharing (QSharedDataPointer)
- Comprehensive error handling and validation
- Modern C++17 implementation
- Full test coverage with QtTest framework

## Requirements

- C++17 compliant compiler
- CMake 3.16 or higher
- Qt6 Core and Test modules

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Running Tests

```bash
cd build
ctest
```

## Usage

### Basic Gadget

```cpp
class Gadget : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(int value READ value WRITE setValue)
public:
    Gadget() : AbstractGadget(new Private) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    int value() const { return d&lt;Private&gt;()-&gt;value; }
    void setValue(int value) { d&lt;Private&gt;()-&gt;value = value; }

private:
    struct Private : public AbstractGadget::Private {
        int value = 0;
        Private *clone() const override { return new Private(*this); }
    };
};
```

### Inheritance Support

```cpp
class SubGadget : public Gadget
{
    Q_GADGET
    Q_PROPERTY(int value1 READ value1 WRITE setValue1)
public:
    SubGadget() : Gadget(new Private) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    int value1() const { return d&lt;Private&gt;()-&gt;value1; }
    void setValue1(int value1) { d&lt;Private&gt;()-&gt;value1 = value1; }

private:
    struct Private : public Gadget::Private {
        int value1 = 0;
        Private *clone() const override { return new Private(*this); }
    };
};
```

### Supported Types

The library supports a wide range of types for properties:

- Basic types: bool, int
- Qt types: QString, QByteArray, QJsonObject
- Container types: QList&lt;T&gt; for supported types
- Enum types with Q_ENUM support
- Nested gadget types and lists of gadgets
- Custom gadget types

### JSON Serialization

```cpp
// Create and set values
Types types;
types.setBoolean(true);
types.setIntegers({1, 2, 3, 4, 5});
types.setString("string");
types.setEnumeration(Types::Enum::debug);

// Serialize to JSON
QJsonObject json = types.toJsonObject();

// Deserialize from JSON
Types other;
if (other.fromJsonObject(json)) {
    // Success
}
```

Example JSON output:
```json
{
    "boolean": true,
    "integers": [1, 2, 3, 4, 5],
    "string": "string",
    "enumeration": "debug",
    "gadget": { "value": 1 },
    "subGadgets": [
        { "value1": 2 },
        { "value1": 3 }
    ]
}
```

## Key Features

1. Memory Safety
   - Implicit sharing using QSharedDataPointer
   - Virtual clone() method for proper copying
   - RAII-based resource management
   - Thread-safe reference counting

2. Type Safety
   - Qt meta-object system integration
   - Compile-time property type checking
   - Enum validation and serialization
   - Container type support

3. Error Handling
   - JSON parsing validation
   - Property type validation
   - Comprehensive error reporting
   - Safe type conversion

4. Build System
   - C++17 support
   - Qt6 integration
   - Proper test framework
   - Installation configuration

## Best Practices

1. Always implement the clone() method in Private structs
2. Use Q_PROPERTY for all public data members
3. Initialize all members in Private structs
4. Check return values from fromJsonObject()
5. Use proper const-correctness in accessors
6. Implement metaObject() for all gadget classes

## License

MIT License - See LICENSE file for details

Copyright (C) 2025 Signal Slot Inc.
