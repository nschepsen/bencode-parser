#ifndef BDECODER_HPP
#define BDECODER_HPP

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
#define MINOR_VERSION "0"

struct BencodeNode
{
private:
    BencodeType type;

    /*
     * TODO:
     *
     * use UNIONS
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

    void put(const std::string& key, const BencodeNode& value)
    {
        if(this->getType() != BDICT)
        {
            throw std::string("Invalid BencodeType");
        }
        this->dict[key] = value;
    }

    void put(const BencodeNode& node)
    {
        if(this->getType() != BLIST)
        {
            throw std::string("Invalid BencodeType");
        }
        this->list.push_back(node);
    }

    void setValue(const std::map<std::string, BencodeNode>& data)
    {
        if(this->getType() != BDICT)
        {
            throw std::string("Invalid BencodeType");
        }
        this->dict = data;
    }

    const std::map<std::string, BencodeNode>& getDict()
    {
        if(this->getType() != BDICT)
        {
            throw std::string("Invalid BencodeType");
        }
        return this->dict;
    }

    void setValue(long long data)
    {
        if(this->getType() != BINT)
        {
            throw std::string("Invalid BencodeType");
        }
        this->integer = data;
    }

    long long getInt()
    {
        if(this->getType() != BINT)
        {
            throw std::string("Invalid BencodeType");
        }
        return this->integer;
    }

    void setValue(const std::vector<BencodeNode>& data)
    {
        if(this->getType() != BLIST)
        {
            throw std::string("Invalid BencodeType");
        }
        this->list = data;
    }

    const std::vector<BencodeNode>& getList()
    {
        if(this->getType() != BLIST)
        {
            throw std::string("Invalid BencodeType");
        }
        return this->list;
    }

    void setValue(const std::string& data)
    {
        if(this->getType() != BSTRING)
        {
            throw std::string("Invalid BencodeType");
        }
        this->string = data;
    }

    const std::string& getString()
    {
        if(this->getType() != BSTRING)
        {
            throw std::string("Invalid BencodeType");
        }
        return this->string;
    }
};

class BDecoder
{
private:
    std::string code; std::string::iterator head;
public:
    BDecoder(char* bencoded)
    :
    code(bencoded),
    head(this->code.begin())
    {
    }
    BencodeNode decode()
    {
        BencodeNode node;

        /*
         * Detects the token's type and parse it
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
            throw std::string("Invalid bencoded Format");
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

#endif /* BDECODER_HPP */
