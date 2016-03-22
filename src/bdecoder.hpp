#ifndef TR_BDECODER_HPP
#define TR_BDECODER_HPP

#include <algorithm>
#include <map>
#include <string>
#include <vector>

enum BencodeType
{
    UNKNOWN,
    BDICT,
    BINT,
    BLIST,
    BSTRING
};

#define MAJOR_VERSION "1"
#define MINOR_VERSION "1"

#define BUILD_VERSION "20160322"

struct BencodeNode
{
private:
    BencodeType type;

    /*
     * TODO:
     *
     * use UNIONS or any Variant DataType
     */

    std::string string;
    long long integer;
    std::map<std::string, BencodeNode> dict;
    std::vector<BencodeNode> list;
public:
    void setType(BencodeType type)
    {
        this->type = type;
    }

    BencodeType getType() const
    {
        return this->type;
    }

    std::string toString()
    {
        std::string s;

        /*
         * This part converts objects of type BencodeNode to std::strings
         */

        switch(this->getType())
        {
        case BencodeType::BDICT:
            s.append("{ ");
            for(auto item : this->getDict())
            {
                s.append
                (
                    item.first
                    +
                    " : "
                    +
                    item.second.toString()
                    +
                    (
                        item.second.isEqual((*this->getDict().rbegin()).second) ? "" : ", "
                    )
                );
            }
            s.append(" }");
            break;
        case BencodeType::BINT:
            s.append(std::to_string(this->getInt()));
            break;
        case BencodeType::BLIST:
            s.append("[");
            for(auto item : this->getList())
            {
                s.append
                (
                    item.toString()
                    +
                    (
                        item.isEqual(this->getList().back()) ? "" : ", "
                    )
                );
            }
            s.append("]");
            break;
        case BencodeType::BSTRING:
            s.append(this->getString());
            break;
        default:
            throw std::string("BencodeType is UNKNOWN");
        }

        return s;
    }

    std::string encode()
    {
        std::string s;

        /*
         * This part of code BEncodes objects of type BencodeNode to std::strings
         */

        switch(this->getType())
        {
        case BencodeType::BDICT:
            s.append("d");
            for(auto item : this->getDict())
            {
                s.append
                    (
                        std::to_string(item.first.size())
                        +
                        ":"
                        +
                        item.first
                        +
                        item.second.encode()
                    );
            }
            s.append("e");
            break;
        case BencodeType::BINT:
            s.append("i" + std::to_string(this->getInt()) + "e");
            break;
        case BencodeType::BLIST:
            s.append("l");
            for(auto item : this->getList())
            {
                s.append(item.encode());
            }
            s.append("e");
            break;
        case BencodeType::BSTRING:
            s.append(std::to_string(this->getString().length()) + ":" + this->getString());
            break;
        default:
            throw std::string("BencodeType is UNKNOWN");
        }

        return s;
    }

    bool isEqual(const BencodeNode& other) const
    {
        if(this->getType() != other.getType())
        {
            return false;
        }
        switch(this->getType())
        {
        case BencodeType::BDICT:

            if(this->getDict().size() != other.getDict().size())
            {
                return false;
            }

            for(auto a = this->getDict().begin(), b = other.getDict().begin(); a != this->getDict().end(); ++a, ++b)
            {
                if((*a).first != (*b).first)
                {
                    return false;
                }
                if(!(*a).second.isEqual((*b).second))
                {
                    return false;
                }
            }

            return true;

        case BencodeType::BINT:

            return this->getInt() == other.getInt();

        case BencodeType::BLIST:

            if(this->getList().size() != other.getList().size())
            {
                return false;
            }
            for(unsigned i = 0; i < this->getList().size(); ++i)
            {
                if(!this->getList()[i].isEqual(other.getList()[i]))
                {
                    return false;
                }
            }

            return true;

        case BencodeType::BSTRING:

            return this->getString() == other.getString();

        default:

            throw std::string("BencodeType is UNKNOWN");
        }
    }

    void put(const std::string& key, const BencodeNode& value)
    {
        if(this->getType() != BDICT)
        {
            throw std::string("BencodeType is UNKNOWN");
        }
        this->dict[key] = value;
    }

    void put(const BencodeNode& node)
    {
        if(this->getType() != BLIST)
        {
            throw std::string("BencodeType is UNKNOWN");
        }
        this->list.push_back(node);
    }

    void setValue(const std::map<std::string, BencodeNode>& data)
    {
        if(this->getType() != BDICT)
        {
            throw std::string("BencodeType is UNKNOWN");
        }
        this->dict = data;
    }

    const std::map<std::string, BencodeNode>& getDict() const
    {
        if(this->getType() != BDICT)
        {
            throw std::string("BencodeType is UNKNOWN");
        }
        return this->dict;
    }

    void setValue(long long data)
    {
        if(this->getType() != BINT)
        {
            throw std::string("BencodeType is UNKNOWN");
        }
        this->integer = data;
    }

    long long getInt() const
    {
        if(this->getType() != BINT)
        {
            throw std::string("BencodeType is UNKNOWN");
        }
        return this->integer;
    }

    void setValue(const std::vector<BencodeNode>& data)
    {
        if(this->getType() != BLIST)
        {
            throw std::string("BencodeType is UNKNOWN");
        }
        this->list = data;
    }

    const std::vector<BencodeNode>& getList() const
    {
        if(this->getType() != BLIST)
        {
            throw std::string("BencodeType is UNKNOWN");
        }
        return this->list;
    }

    void setValue(const std::string& data)
    {
        if(this->getType() != BSTRING)
        {
            throw std::string("BencodeType is UNKNOWN");
        }
        this->string = data;
    }

    const std::string& getString() const
    {
        if(this->getType() != BSTRING)
        {
            throw std::string("BencodeType is UNKNOWN");
        }
        return this->string;
    }
};

class BDecoder
{
private:

    std::string code; std::string::iterator head;

public:

    BDecoder(char* bencoded, int length)
    :
    code(bencoded, length),
    head(this->code.begin())
    {
    }

    BDecoder(std::string& bencoded)
    :
    code(bencoded),
    head(this->code.begin())
    {
    }

    BencodeNode decode()
    {
        BencodeNode node;

        /*
         * Detects the token's type and parses it
         */

        switch(*this->head)
        {
        case 'd':
            node.setType(BencodeType::BDICT);
            this->head++;
            while(*this->head != 'e')
            {
                std::string s(this->parseString());
                node.put
                (
                    s, this->decode()
                );
            }
            if(*(this->head) == 'e') head++;
            break;
        case 'i':
            node.setType(BencodeType::BINT);
            node.setValue
            (
                this->parseInteger()
            );
            break;
        case 'l':
            node.setType(BencodeType::BLIST);
            this->head++;
            while(*this->head != 'e')
            {
                node.put(this->decode());
            }
            if(*(this->head) == 'e') head++;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            node.setType(BencodeType::BSTRING);
            node.setValue(this->parseString());
            break;
        default:
            throw std::string("Invalid Token");
        }

        return node;
    }

    std::string parseString()
    {
        long long offset(this->parseInteger(':'));

        std::string::iterator begin
        (
            this->head
        );

        return std::string(begin, this->head += offset);
    }

    long long parseInteger(char terminal = 'e')
    {
        std::string::iterator begin(this->head + (*head == 'i' ? 1 : 0));

        this->head = std::find
                (
                    begin, this->code.end(), terminal
                ) + 1;

        return std::stoll(std::string(begin, this->head - 1));
    }
};

#endif /* TR_BDECODER_HPP */
