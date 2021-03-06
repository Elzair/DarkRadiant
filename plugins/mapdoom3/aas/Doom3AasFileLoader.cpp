#include "Doom3AasFileLoader.h"

#include "itextstream.h"

#include "parser/DefTokeniser.h"
#include "string/convert.h"
#include "Doom3AasFile.h"

namespace map
{

namespace
{
    const float DEWM3_AAS_VERSION = 1.07f;
}

const std::string& Doom3AasFileLoader::getAasFormatName() const
{
    static std::string _name = "Doom 3 AAS";
	return _name;
}

const std::string& Doom3AasFileLoader::getGameType() const
{
    static std::string _gameType = "doom3";
	return _gameType;
}

bool Doom3AasFileLoader::canLoad(std::istream& stream) const
{
    // Instantiate a tokeniser to read the first few tokens
	parser::BasicDefTokeniser<std::istream> tok(stream);

	try
	{
        parseVersion(tok);
	}
	catch (parser::ParseException&)
	{
        return false;
    }
	catch (boost::bad_lexical_cast&)
	{
        return false;
    }

	return true;
}

IAasFilePtr Doom3AasFileLoader::loadFromStream(std::istream& stream)
{
    Doom3AasFilePtr aasFile = std::make_shared<Doom3AasFile>();

    // We assume that the stream is rewound to the beginning

    // Instantiate a tokeniser to read the version tag
	parser::BasicDefTokeniser<std::istream> tok(stream);

    try
	{
        // File header
        parseVersion(tok);

        // Checksum (will throw if the stirng conversion fails)
        string::convert<long>(tok.nextToken());

        aasFile->parseFromTokens(tok);
	}
	catch (parser::ParseException& ex)
	{
        rError() << "Failure parsing AAS file: " << ex.what() << std::endl;
        return IAasFilePtr();
    }
	catch (boost::bad_lexical_cast& ex)
	{
        rError() << "Conversion error while parsing AAS file: " << ex.what() << std::endl;
        return IAasFilePtr();
    }

    return aasFile;
}

void Doom3AasFileLoader::parseVersion(parser::DefTokeniser& tok) const
{
    // Require a "Version" token
    tok.assertNextToken("DewmAAS");

	// Require specific version, return true on success 
    if (string::convert<float>(tok.nextToken()) != DEWM3_AAS_VERSION)
    {
        throw parser::ParseException("AAS File version mismatch");
    }
}

const std::string& Doom3AasFileLoader::getName() const
{
	static std::string _name("Doom3AasFileLoader");
	return _name;
}

const StringSet& Doom3AasFileLoader::getDependencies() const
{
	static StringSet _dependencies;

	if (_dependencies.empty())
	{
        _dependencies.insert(MODULE_AASFILEMANAGER);
	}

	return _dependencies;
}

void Doom3AasFileLoader::initialiseModule(const ApplicationContext& ctx)
{
	rMessage() << getName() << ": initialiseModule called." << std::endl;

	// Register ourselves as aas format
    GlobalAasFileManager().registerLoader(shared_from_this());
}

void Doom3AasFileLoader::shutdownModule()
{
	// Unregister now that we're shutting down
	GlobalAasFileManager().unregisterLoader(shared_from_this());
}

}
