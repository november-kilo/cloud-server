#include <Maths.h>
#include <Terminal.h>

#define VOP_ADD 0
#define VOP_SUB 1
#define VOP_MUL 2
#define VOP_DIV 3

inherit Number;
inherit Terminal;

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

Vector scale(float f) {
    Iterator iterator;
    Number *ans;
    int sz;

    sz = sizeof(components);
    ans = allocate(sz);
    iterator = new IntIterator(0, sz - 1);

    while (!iterator->end()) {
        ans[iterator->next()] = components[iterator->current()] * f;
    }

    return new Vector(ans);
}

Number norm(void) {
    Iterator iterator;
    Number total;
    int sz;

    sz = sizeof(components);
    iterator = new IntIterator(0, sz -1);
    total = new Number(0);

    while (!iterator->end()) {
        total += components[iterator->next()] * components[iterator->current()];
    }

    return total->_sqrt();
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

    if (size() == 3 && that->size() == 3) {
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

    error("Vector: cross product defined only for 3D vectors");
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
            return cross(value);
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

mapping angleFrom(Vector a) {
    Number d, sz;
    float r;
    mapping ans;

    d = dot(a);
    sz = norm() * a->norm();
    r = acos(d->toFloat() / sz->toFloat());

    ans = ([ ]);
    ans["rad"] = r;
    ans["deg"] = radianToDegree(r);

    return ans;
}

private string componentToString(Number f, string x) {
    return "(" + f->toString() + ")" + x;
}

string toString(varargs int projected) {
    Iterator iterator;
    string result;
    int sz;

    sz = sizeof(components);

    if (projected) {
        if (sz == 3) {
            return componentToString(components[0], "i") +
                   " + " + componentToString(components[1], "j") +
                   " + " + componentToString(components[2], "k");
        }
        error("Vector: 3D vectors only.");
    }

    if (sz == 1) {
        return "< " + components[0]->toString() + " >";
    }

    if (sz == 2) {
        return "< " + components[0]->toString() + ", " + components[1]->toString() + " >";
    }

    iterator = new IntIterator(0, sizeof(components) - 2);
    result = "";

    while (!iterator->end()) {
        result += "" + components[iterator->next()]->toString() + ", ";
    }
    result += "" + components[sz - 1]->toString() + " ";

    return "< " + result + ">";
}

string toNorm(void) {
    return vpipeDouble() + norm()->toFloat() + vpipeDouble();
}

string unitVectorInDirectionOf(void) {
    return toString(1) + " / " + norm()->toString();
}

string unitVectorOrthogonalTo(Vector a) {
    Vector crossOfThisAndThat;

    crossOfThisAndThat = cross(a);

    return crossOfThisAndThat->toString(1) + " / " + crossOfThisAndThat->norm()->toString();
}

mapping directionCosines(void) {
    Number r, a, b, c;
    mapping cosines;

    r = norm();
    cosines = ([ ]);
    a = new Number(acos(components[0]->toFloat() / r->toFloat()));
    b = new Number(acos(components[1]->toFloat() / r->toFloat()));
    c = new Number(acos(components[2]->toFloat() / r->toFloat()));

    cosines["a"] = ([ "a": components[0] / r, "rad": a, "deg": a->toDegree() ]);
    cosines["b"] = ([ "b": components[1] / r, "rad": b, "deg": b->toDegree() ]);
    cosines["c"] = ([ "c": components[2] / r, "rad": c, "deg": c->toDegree() ]);

    return cosines;
}

mapping polar(void) {
    mapping ans;
    Number r, theta;

    ans = ([ ]);
    r = norm();
    theta = new Number(atan(components[1]->toFloat() / components[0]->toFloat()));

    ans["radial"] = r;
    ans["angular"] = ([ "rad": theta, "deg": theta->toDegree() ]);

    return ans;
}

mapping cylindrical(void) {
    mapping p, ans;

    p = polar();
    ans = ([ ]);
    ans["radial"] = p["radial"];
    ans["azimuthal"] = p["angular"];
    ans["vertical"] = components[2];

    return ans;
}

mapping spherical(void) {
    mapping ans;
    Number p;

    ans = cylindrical();
    p = new Number(acos(components[2]->toFloat() / ans["radial"]->toFloat()));
    ans["polar"] = ([ "rad": p, "deg": p->toDegree() ]);
    ans["vertical"] = nil;

    return ans;
}
