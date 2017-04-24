// Copyright 2016 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.
/**
 * \class document::ArrayFieldValue
 * \ingroup fieldvalue
 *
 * \brief A representation of an array of fieldvalues of specific types.
 *
 * A field value representing an array of other fieldvalues of a given type.
 *
 * \see datatype.h
 * \see field.h
 */
#pragma once

#include "collectionfieldvalue.h"
#include <vespa/document/datatype/arraydatatype.h>

namespace document {

class ArrayFieldValue : public CollectionFieldValue {
private:
    IArray::UP _array;

    bool addValue(const FieldValue&) override;
    bool containsValue(const FieldValue& val) const override;
    bool removeValue(const FieldValue& val) override;
    IteratorHandler::ModificationStatus iterateSubset(
            int startPos, int endPos, const vespalib::stringref & variable,
            PathRange nested,
            IteratorHandler& handler) const;
    IteratorHandler::ModificationStatus onIterateNested(PathRange nested, IteratorHandler & handler) const override;
public:
    typedef IArray::const_iterator const_iterator;
    typedef IArray::iterator iterator;
    typedef std::unique_ptr<ArrayFieldValue> UP;

    /**
     * @param arrayType Type of the array. Must be an ArrayDataType, but does
     *                  not enforce type compile time so it will be easier to
     *                  create instances using field's getDataType().
     */
    ArrayFieldValue(const DataType &arrayType);
    ArrayFieldValue(const ArrayFieldValue&);
    ~ArrayFieldValue();

    ArrayFieldValue& operator=(const ArrayFieldValue&);

    const FieldValue& operator[](uint32_t index) const { return array()[index]; }
    FieldValue& operator[](uint32_t index) { return array()[index]; }

    void accept(FieldValueVisitor &visitor) override { visitor.visit(*this); }
    void accept(ConstFieldValueVisitor &visitor) const override { visitor.visit(*this); }

    void append(FieldValue::UP value) { _array->push_back(*value); }
    void remove(uint32_t index);
    bool remove(const FieldValue& val) { return removeValue(val); }

    bool isEmpty() const override { return _array->empty(); }
    size_t size() const override { return _array->size(); }
    void clear() override { _array->clear(); }
    void reserve(size_t sz) { _array->reserve(sz); }
    void resize(size_t sz) { _array->resize(sz); }

    FieldValue& assign(const FieldValue&) override;
    ArrayFieldValue* clone() const override { return new ArrayFieldValue(*this); }
    int compare(const FieldValue&) const override;
    void printXml(XmlOutputStream& out) const override;
    void print(std::ostream& out, bool verbose, const std::string& indent) const override;
    bool hasChanged() const override;
    void swap(ArrayFieldValue & other) { _array.swap(other._array); }

        // Iterator functionality
    const_iterator begin() const { return array().begin(); }
    const_iterator end() const { return array().end(); }
    iterator begin() { return array().begin(); }
    iterator end() { return array().end(); }

    DECLARE_IDENTIFIABLE_ABSTRACT(ArrayFieldValue);
private:
    const IArray & array() const { return *_array; }
    IArray & array() { return *_array; }
};

} // document

