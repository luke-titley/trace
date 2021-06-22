//Copywrite Luke Titley 2015
#ifndef LSD
#define LSD

#include <cstdlib>
#include <vector>

namespace lsd
{
class Node;
//------------------------------------------------------------------------------
// Stage
//------------------------------------------------------------------------------
class Stage
{
public:
    inline Stage();
    template<typename T>
    T * alloc(); // TODO LT: A placeholder for a smarter memory allocator
    template<typename T, typename Arg0>
    T * alloc(Arg0 arg0); // TODO LT: A placeholder for a smarter memory allocator
    template<typename T>
    T * allocArray(const size_t size); // TODO LT: A placeholder for a smarter memory allocator
    inline
    void setRoot(Node * root);
    inline
    Node * getRoot() const;
    inline
    bool empty() const;

private:
    Node * m_root;
};

//------------------------------------------------------------------------------
Stage::Stage():
    m_root(0)
{}

//------------------------------------------------------------------------------
template<typename T>
T * Stage::alloc()
{
    return new T;
};

//------------------------------------------------------------------------------
template<typename T, typename Arg0>
T * Stage::alloc(Arg0 arg0)
{
    return new T(arg0);
};

//------------------------------------------------------------------------------
template<typename T>
T * Stage::allocArray(const size_t size)
{
    return new T[size];
};

//------------------------------------------------------------------------------
void Stage::setRoot(Node * root)
{
    m_root = root;
}

//------------------------------------------------------------------------------
Node * Stage::getRoot() const
{
    return m_root;
}

//------------------------------------------------------------------------------
bool Stage::empty() const
{
    return m_root == 0;
}

//------------------------------------------------------------------------------
// Node
//------------------------------------------------------------------------------
class Node
{
    bool m_isXform;
    char * m_name;
public:
    inline Node(bool isXform, char * name):
                m_isXform(isXform),
                m_name(name)
    {}
    inline bool isXform() const {return m_isXform;}
    inline bool isPolyMesh() const {return !m_isXform;}
    template<typename T>
    T * as(){return static_cast<T*>(this);}
    template<typename T>
    const T * as() const {return static_cast<const T *>(this);}

    inline const char * getName() const {return m_name;}
};

//------------------------------------------------------------------------------
// Xform
//------------------------------------------------------------------------------
class Xform : public Node
{
public:
    typedef std::vector<Node*> Children;
    Children * m_children;
    inline Xform(char * name):
                 Node(true, name),
                 m_children(0)
    {}
};

//------------------------------------------------------------------------------
// TriValue
//------------------------------------------------------------------------------
template<typename T>
class TriValue
{
public:
    union
    {
        T value[3];
        struct
        {
            T x;
            T y;
            T z;
        };
        struct
        {
            T a;
            T b;
            T c;
        };
    };
};

typedef TriValue<float> Vertex;
typedef TriValue<size_t> Triangle;

//------------------------------------------------------------------------------
// PolyMesh
//------------------------------------------------------------------------------
class PolyMesh : public Node
{
public:
    typedef std::vector<Vertex> Verticies;
    typedef std::vector<Triangle> Triangles;

    Verticies * m_verticies;
    Triangles * m_triangles;

    PolyMesh(char * name):
        Node(false, name),
        m_verticies(0),
        m_triangles(0)
    {}
};

//------------------------------------------------------------------------------
// PolyMesh
//------------------------------------------------------------------------------
union Variant
{
    Node * m_node;
    Xform::Children * m_children;
    PolyMesh::Verticies * m_verticies;
    PolyMesh::Triangles * m_triangles;
    char * m_string;
    Vertex m_vertex;
    Triangle m_triangle;
    double m_double;
    unsigned int m_int;
};

//------------------------------------------------------------------------------
// StageIterator
//------------------------------------------------------------------------------
class StageIterator
{
    typedef std::vector<Node *> NodeStack;
private:
    const Stage & m_stage;
    Node * m_current;
    NodeStack m_nodeStack;
    bool m_recurseIntoChildren;
public:
    inline StageIterator(const Stage & stage);
    inline void begin();
    inline bool next();
    inline void recurseIntoChildren(bool value);
    inline const Node & operator*() const;
};

//------------------------------------------------------------------------------
StageIterator::StageIterator(const Stage & stage):
    m_stage(stage),
    m_current(0),
    m_recurseIntoChildren(false)
{}

//------------------------------------------------------------------------------
void StageIterator::begin()
{
    m_nodeStack.clear();
    m_current = 0;
    m_recurseIntoChildren = false;
    if(!m_stage.empty())
    {
        m_nodeStack.push_back(m_stage.getRoot());
    }
}

//------------------------------------------------------------------------------
bool StageIterator::next()
{
    // Push children onto the stack
    if(m_current != 0 && m_recurseIntoChildren)
    {
        m_recurseIntoChildren = false;
        if(m_current->isXform())
        {
            Xform * xform = m_current->as<Xform>();
            if(xform->m_children != 0)
            {
                Xform::Children::iterator it = xform->m_children->begin();
                for(; it != xform->m_children->end(); ++it)
                {
                    m_nodeStack.push_back(*it);
                }
            }
        }
    }

    // Opt out if the stack is empty
    if(m_nodeStack.empty())
    {
        return false;
    }
    // Keep a track of our current node
    m_current = m_nodeStack.back();
    m_nodeStack.pop_back();

    return true;
}

//------------------------------------------------------------------------------
void StageIterator::recurseIntoChildren(bool value)
{
    m_recurseIntoChildren = value;
}

//------------------------------------------------------------------------------
const Node & StageIterator::operator*() const
{
    return *m_current;
}

} // namespace lsd

#endif
