// MIT license
//
// Copyright 2023 Per Nilsson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _JSTR_HH_
#define _JSTR_HH_

#include <memory>
#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp>

namespace Jstr {
namespace Xpath {

// Node    
class Node {
public:
    Node();
    Node(const std::string& name, const nlohmann::json& json, int64_t i = -1);
    Node(const Node* parent, const std::string& name, const nlohmann::json& json, int64_t i = -1);
    Node(const Node& node);
    Node& operator=(const Node& node);
    Node getRoot() const;
    const Node* getParent() const;
    const nlohmann::json& getJson() const;
    bool isValue() const;
    double getNumber() const;
    bool getBool() const;
    std::string getString() const;
    const std::string& getLocalName() const;
    bool isArrayChild() const;
    void getAncestors(std::vector<Node>& result) const;
    void getChild(const std::string& name, std::vector<Node>& result) const;
    void getChildren(std::vector<Node>& result) const;
    void getSubTreeNodes(std::vector<Node>& result) const;
    void search(const std::string& name, std::vector<Node>& result) const;
    bool operator==(const Node& r) const;
private:
    void assign(const Node& node);
    std::unique_ptr<const Node> _parent;
    std::string _name;
    const nlohmann::json* _json;
    int64_t _i;
};

inline
std::ostream&
operator<<(std::ostream& os, const Node& n) {
    os << n.getJson().dump();
    return os;
}

inline
std::ostream&
operator<<(std::ostream& os, const std::vector<Node>& ns) {
    os << "[";
    bool first(true);
    std::string separator;
    for (const Node& n : ns) {
        os << separator << n;
        if (first) {
            separator = ", ";
        }
    }
    os << "]";
    return os;
}


// Value
class Value {
public:
    enum Type {
        Number,
        Bool,
        String,
        NodeSet
    };
    Value();
    Value(const Value& xd);
    Value(Value&& xd);
    Value(double d);
    Value(bool b);
    Value(const char* s);
    Value(const std::string& s);
    Value(const std::string& name, const nlohmann::json& json);
    Value(const Node& node);
    Value(const std::vector<Node>& ns);
    Value& operator=(const Value& xd);
    Value& operator=(Value&& xd);
    ~Value();
    Type getType() const;
    /**
     * Returns an indication if this is a primitive value or not.
     * @return true if this object represents a literal or a single node with a value.
     */
    bool isValue() const;
    double getNumber() const;
    bool getBool() const;       // TODO Rename to boolean
    /**
     * Primitiv values are converted to strings. For node sets the "string value"
     * of the first node is returned. If the node set is empty the empty string is
     * returned.
     * @return a string representation of the value.
     */
    std::string getString() const;
    /**
     * Returns the XML "string value" of the data.
     * Primitive values are interpreted as XML text nodes.
     * Objects and arrays are mapped to elements.
     * "The string-value of an element node is the concatenation of the string-values of
     *  all text node descendants of the element node in document order."
     * @return a string representation of the value.
     */
    std::string getStringValue() const;
    const Node& getNode(size_t pos) const;
    const std::vector<Node>& getNodeSet() const;
    Value getNodeSetSize() const;
    Value getLocalName() const;
    Value getRoot() const;
    Value nodeSetUnion(const Value& v) const;
    bool operator==(const Value& v) const;
    bool operator!=(const Value& v) const;
    bool operator<(const Value& v) const;
    bool operator<=(const Value& v) const;
    bool operator>(const Value& v) const;
    bool operator>=(const Value& v) const;
private:
    void checkOrderingRelationArgs(const Value& v) const;
    void assign(const Value& v);
    void exchange(Value&& v);
    void clear();
    Type _type;
    union Data {
        double n;
        bool b;
        std::string* s;
        std::vector<Node>* ns;
    } _d;
};

inline
bool
operator==(const Value& v, double d) {
    const std::vector<Node>& ns = v.getNodeSet();
    for (const Node& l : v.getNodeSet()) {
        if (l.getNumber() == d) {
            return true;
        }
    }
    return false;
}

inline
bool
operator==(const Value& v, const std::string& s) {
    const std::vector<Node>& ns = v.getNodeSet();
    for (const Node& l : v.getNodeSet()) {
        const std::string& ls = l.getString();
        if (ls == s) {
            return true;
        }
    }
    return false;
}

inline
bool
operator==(const Value& v, bool b) {
    for (const Node& l : v.getNodeSet()) {
        if (l.getBool() == b) {
            return true;
        }
    }
    return false;
}

inline
bool
operator!=(const Value& v, double d) {
    const std::vector<Node>& ns = v.getNodeSet();
    for (const Node& l : v.getNodeSet()) {
        if (l.getNumber() != d) {
            return true;
        }
    }
    return false;
}

inline
bool
operator!=(const Value& v, const std::string& s) {
    const std::vector<Node>& ns = v.getNodeSet();
    for (const Node& l : v.getNodeSet()) {
        const std::string& ls = l.getString();
        if (ls != s) {
            return true;
        }
    }
    return false;
}

inline
bool
operator!=(const Value& v, bool b) {
    for (const Node& l : v.getNodeSet()) {
        if (l.getBool() != b) {
            return true;
        }
    }
    return false;
}

inline
std::ostream&
operator<<(std::ostream& os, const Value& v) {
    Value::Type type = v.getType();
    switch(type) {
    case Value::Number:
        os << "Number: " << v.getNumber();
        break;
    case Value::Bool:
        os << "Bool: " << v.getBool();
        break;
    case Value::String:
        os << "String: " << v.getString();
        break;
    case Value::NodeSet: {
        os << v.getNodeSet();
    }
        break;
    default:
        os << "Uknown type";
        break;
    }
    return os;
}

// Env
class Env {
public:
    /**
     * Creates an environment with a context node supplied as a json object.
     * The json object is a "top level" entity in the data tree.
     * @param json a top level json entity.
     */
    Env(const nlohmann::json& json);
    /**
     * Creates an environment with a context node. The context node must be either
     * a primitivite value (number, boolean or string) or a node set with one node.
     * @param context the current context of the environment.
     */
    Env(const Value& context);
    const Value& getCurrent() const;
    void addVariable(const std::string& name, const Value& v);
    const Value& getVariable(const std::string& name) const;
private:
    std::map<std::string, Value> _vals;
    Value _context;
};

class Expr;

class Expression {
public:
    Expression(const std::string& s);
    Expression(const Expression& expr) = delete;
    Expression& operator=(const Expression& expr) = delete;
    ~Expression();
    Value eval(const Env& env) const;
private:
    const Expr* _expr;
};
    
Value
eval(const std::string& xpath, const nlohmann::json& json);

}

 namespace Schematron {

bool
eval(const nlohmann::json& schematron, const nlohmann::json& data, std::ostream& out);

}
}


#endif
