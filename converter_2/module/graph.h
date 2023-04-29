#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <memory>
#include <sstream>

struct ConstructHelper;
struct Graph {
    struct Unit {
        std::string id, name;
        virtual ~Unit() = default;
    };

    struct Edge;
    struct Node : Unit {
        std::vector<std::weak_ptr<Edge>> outs, inns;
    };

    struct Edge : Unit {
        std::weak_ptr<Node> beg, end;
    };

public:
    Graph();
    virtual ~Graph() = default;

    // enough.
    virtual void read_puml(std::istream&) = 0;
    virtual void write_json(std::ostream&) = 0;

public:
    /* readonly */
    std::vector<std::shared_ptr<Node>> nodes; // &
    std::vector<std::shared_ptr<Edge>> edges;

    // TODO: часть методов можно спрятать в cpp?
protected:
    bool try_directive(const std::string&) const;
    bool try_skinparam(const std::string&) const;
    bool try_direction(const std::string&) const;
    bool try_whitespaces(const std::string&) const;

protected:
    bool try_beg_grouping(const std::string&) const;
    bool try_end_curly_brace(const std::string&) const;

protected:
    bool try_one_note(const std::string&) const;
    bool try_beg_note_with_id(const std::string&) const;
    bool try_beg_multi_note(const std::string&) const;
    bool try_end_multi_note(const std::string&) const;
    bool try_note(const std::string&, std::istream&);

protected:
    bool try_one_comment(const std::string&) const;
    bool try_beg_multi_comment(const std::string&) const;
    bool try_end_multi_comment(const std::string&) const;
    bool try_comment(const std::string&, std::istream&);

protected:
    virtual bool try_any(const std::string&, std::istream&);
    virtual bool try_node(const std::string&, std::istream&) = 0;
    virtual bool try_connection(const std::string&, std::istream&) = 0;
    virtual bool try_grouping(const std::string&, std::istream&);

protected:
    const std::shared_ptr<ConstructHelper> m_ch;
    std::string read_line(std::istream&);
};

#endif // GRAPH_H

