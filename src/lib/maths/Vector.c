#include <Maths.h>

#define VOP_ADD 0
#define VOP_SUB 1
#define VOP_MUL 2
#define VOP_DIV 3

inherit Number;

private Number *components;

static void create(Number *components) {
    ::components = components;
}

Number *components(void) {
    return components;
}

int size(void) {
    return sizeof(components);
}

Number get(int index) {
    if (size() == 0) {
        return nil;
    }

    if (index < 0 || index >= size()) {
        error("Vector: Invalid index");
    }

    return components[index];
}

string toString() {
    string str;
    int i, sz;

    str = "{";
    for (i = 0; i < size(); i++) {
        str += " " + get(i)->toString();
        if (i < size() - 1) {
            str += ",";
        }
    }
    str += " }";
    return str;
}

Vector negate(void) {
    Number *negated;
    Iterator i;

    if (size() == 0) {
        return new Vector(({ }));
    }

    negated = allocate(size());
    i = new IntIterator(0, size() - 1);

    while (!i->end()) {
        negated[i->next()] = -components[i->current()];
    }

    return new Vector(negated);
}

Number dot(Vector that) {
    Number x;
    Iterator iterator;

    if (size() != that->size()) {
        error("Vector: size mismatch for dot product");
    }

    iterator = new IntIterator(0, size() - 1);
    x = new Number(0);
    while (!iterator->end()) {
        x = x + get(iterator->next()) * that->get(iterator->current());
    }

    return x;
}

Vector cross(Vector that) {
    Number ux, uy, uz;
    Number vx, vy, vz;
    Number x, y, z;

    if (size() != 3 && that->size() != 3) {
        error("Vector: cross product defined only for 3D vectors");
    }

    ux = get(0);
    uy = get(1);
    uz = get(2);
    vx = that->get(0);
    vy = that->get(1);
    vz = that->get(2);

    x = uy * vz - uz * vy;
    y = uz * vx - ux * vz;
    z = ux * vy - uy * vx;

    return new Vector(({ x, y, z }));
}

private string getOpStr(int op) {
    switch (op) {
        case VOP_SUB:
            return "subtraction";
        case VOP_MUL:
            return "multiplication";
        case VOP_DIV:
            return "division";
        case VOP_ADD:
        default:
            return "addition";
    }
}

private Vector vectorOperation(Vector value, varargs int op) {
    Iterator i;
    Number *result;

    if (value->size() == 0) {
        return new Vector(components);
    }

    if (size() == 0) {
        return new Vector(value->components());
    }

    if (size() != value->size()) {
        error("Vector: size mismatch for " + getOpStr(op));
    }

    i = new IntIterator(0, size() - 1);
    result = allocate(size());

    while (!i->end()) {
        switch (op) {
            case VOP_SUB:
                result[i->next()] = get(i->current()) - value->get(i->current());
                break;
            case VOP_ADD:
            default:
                result[i->next()] = get(i->current()) + value->get(i->current());
        }
    }

    return new Vector(result);
}

private Vector elementwiseOperation(Number value, int op) {
    Iterator i;
    Number *result;

    i = new IntIterator(0, size() - 1);
    result = allocate(size());

    while (!i->end()) {
        switch (op) {
            case VOP_SUB:
                result[i->next()] = components[i->current()] - value;
                break;
            case VOP_MUL:
                result[i->next()] = components[i->current()] * value;
                break;
            case VOP_DIV:
                result[i->next()] = components[i->current()] / value;
                break;
            case VOP_ADD:
            default:
                result[i->next()] = components[i->current()] + value;
        }
    }

    return new Vector(result);
}

static Vector operator+ (mixed value) {
    if (typeof(value) == T_OBJECT) {
        if (value <- Vector) {
            return vectorOperation(value, VOP_ADD);
        }

        if (value <- Number) {
            return elementwiseOperation(value, VOP_ADD);
        }
    }

    error("Vector: invalid operand for addition");
}

static Vector operator- (varargs mixed value) {
    if (value == nil) {
        return negate();
    }

    if (typeof(value) == T_OBJECT) {
        if (value <- Vector) {
            return vectorOperation(value, VOP_SUB);
        }

        if (value <- Number) {
            return elementwiseOperation(value, VOP_SUB);
        }
    }

    error("Vector: invalid operand for subtraction");
}

static Vector operator* (mixed value) {
    if (typeof(value) == T_OBJECT) {
        if (value <- Vector) {
            if (size() == 3 && value->size() == 3) {
                return cross(value);
            }
            error("Vector: vector product defined only for 3D vectors");
        }

        if (value <- Number) {
            return elementwiseOperation(value, VOP_MUL);
        }
    }

    error("Vector: invalid operand for multiplication");
}

static Vector operator/ (mixed value) {
    if (typeof(value) == T_OBJECT) {
        if (value <- Vector) {
            error("Vector: vector division is not uniquely defined");
        }

        if (value <- Number) {
            return elementwiseOperation(value, VOP_DIV);
        }
    }

    error("Vector: invalid operand for division");
}
