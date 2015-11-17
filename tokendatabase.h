#ifndef TOKENDATABASE_H
#define TOKENDATABASE_H

#include <vector>
#include <wx/thread.h>
#include <wx/string.h>

template<typename _Tp> class TreeMap;
class wxString;
typedef int FileId;
typedef int TokenId;

typedef enum _TokenType
{
    TokenType_Unknown = 0,
    TokenType_FuncDecl  = 1<<0,
    TokenType_VarDecl   = 1<<1,
    TokenType_ParmDecl  = 1<<2,
    TokenType_ScopeDecl = 1<<3,

}TokenType;

struct AbstractToken
{
    AbstractToken( TokenType typ, FileId fId, int ln, int col, wxString displayName, unsigned tknHash) :
        type(typ), fileId(fId), line(ln), column(col), displayName(displayName.c_str()), tokenHash(tknHash) {}
    AbstractToken( const AbstractToken& other ) :
        type(other.type), fileId(other.fileId), line(other.line), column(other.column), displayName( other.displayName.c_str()), tokenHash(other.tokenHash) {}

    TokenType type;
    FileId fileId;
    int line;
    int column;
    wxString displayName;
    unsigned tokenHash;
};

class TokenDatabase
{
public:
    TokenDatabase();
    ~TokenDatabase();

    FileId GetFilenameId(const wxString& filename);
    wxString GetFilename(FileId fId);
    TokenId GetTokenId(const wxString& identifier, unsigned tokenHash); // returns wxNOT_FOUND on failure
    TokenId InsertToken(const wxString& identifier, const AbstractToken& token); // duplicate tokens are discarded
    AbstractToken GetToken(TokenId tId);
    std::vector<TokenId> GetTokenMatches(const wxString& identifier);
    std::vector<TokenId> GetFileTokens(FileId fId);

    void Shrink();

private:

    TreeMap<AbstractToken>* m_pTokens;
    TreeMap<int>* m_pFileTokens;
    TreeMap<wxString>* m_pFilenames;
    wxMutex m_Mutex;
};

#endif // TOKENDATABASE_H
