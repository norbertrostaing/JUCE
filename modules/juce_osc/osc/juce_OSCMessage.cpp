/*
  ==============================================================================

   This file is part of the JUCE framework.
   Copyright (c) Raw Material Software Limited

   JUCE is an open source framework subject to commercial or open source
   licensing.

   By downloading, installing, or using the JUCE framework, or combining the
   JUCE framework with any other source code, object code, content or any other
   copyrightable work, you agree to the terms of the JUCE End User Licence
   Agreement, and all incorporated terms including the JUCE Privacy Policy and
   the JUCE Website Terms of Service, as applicable, which will bind you. If you
   do not agree to the terms of these agreements, we will not license the JUCE
   framework to you, and you must discontinue the installation or download
   process and cease use of the JUCE framework.

   JUCE End User Licence Agreement: https://juce.com/legal/juce-8-licence/
   JUCE Privacy Policy: https://juce.com/juce-privacy-policy
   JUCE Website Terms of Service: https://juce.com/juce-website-terms-of-service/

   Or:

   You may also use this code under the terms of the AGPLv3:
   https://www.gnu.org/licenses/agpl-3.0.en.html

   THE JUCE FRAMEWORK IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL
   WARRANTIES, WHETHER EXPRESSED OR IMPLIED, INCLUDING WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

OSCMessage::OSCMessage (const OSCAddressPattern& ap) noexcept  : addressPattern (ap)
{
}

//==============================================================================
void OSCMessage::setAddressPattern (const OSCAddressPattern& ap) noexcept
{
    addressPattern = ap;
}

OSCAddressPattern OSCMessage::getAddressPattern() const noexcept
{
    return addressPattern;
}

#if JUCE_IP_AND_PORT_DETECTION 
String OSCMessage::getSenderIPAddress() const noexcept
{
    return senderIPAddress;
}

void OSCMessage::setSenderIPAddress(const String& ip) noexcept
{
    senderIPAddress = ip;
}

int OSCMessage::getSenderPortNumber() const noexcept
{
    return senderPortNumber;
}
void OSCMessage::setSenderPortNumber(int port) noexcept
{
    senderPortNumber = port;
}
#endif 
//==============================================================================
int OSCMessage::size() const noexcept
{
    return arguments.size();
}

bool OSCMessage::isEmpty() const noexcept
{
    return arguments.isEmpty();
}

OSCArgument& OSCMessage::operator[] (const int i) noexcept
{
    return arguments.getReference (i);
}

const OSCArgument& OSCMessage::operator[] (const int i) const noexcept
{
    return arguments.getReference (i);
}

OSCArgument* OSCMessage::begin() noexcept
{
    return arguments.begin();
}

const OSCArgument* OSCMessage::begin() const noexcept
{
    return arguments.begin();
}

OSCArgument* OSCMessage::end() noexcept
{
    return arguments.end();
}

const OSCArgument* OSCMessage::end() const noexcept
{
    return arguments.end();
}

void OSCMessage::clear()
{
    arguments.clear();
}

//==============================================================================
void OSCMessage::addInt32 (int32 value)             { arguments.add (OSCArgument (value)); }
void OSCMessage::addInt64 (int64 value)             { arguments.add (OSCArgument (value)); }
void OSCMessage::addFloat32 (float value)           { arguments.add (OSCArgument (value)); }
void OSCMessage::addDouble (double value)           { arguments.add (OSCArgument (value)); }
void OSCMessage::addString (const String& value)    { arguments.add (OSCArgument (value)); }
void OSCMessage::addBlob (MemoryBlock blob)         { arguments.add (OSCArgument (std::move (blob))); }
void OSCMessage::addColour (OSCColour colour)       { arguments.add (OSCArgument (colour)); }
void OSCMessage::addNil()                           { arguments.add (OSCArgument (OSCTypes::nil)); }
void OSCMessage::addImpulse()                       { arguments.add (OSCArgument (OSCTypes::impulse)); }
void OSCMessage::addBool (bool value)               { arguments.add (OSCArgument (value)); }
void OSCMessage::addArgument (OSCArgument arg)      { arguments.add (arg); }


//==============================================================================
//==============================================================================
#if JUCE_UNIT_TESTS

class OSCMessageTests final : public UnitTest
{
public:
    OSCMessageTests()
        : UnitTest ("OSCMessage class", UnitTestCategories::osc)
    {}

    void runTest() override
    {
        beginTest ("Basic usage");
        {
            OSCMessage msg ("/test/param0");
            expectEquals (msg.size(), 0);
            expect (msg.getAddressPattern().toString() == "/test/param0");

            const int numTestArgs = 11;

            const int testInt = 42;
            const int64 testInt64 = 64;
            const float testFloat = 3.14159f;
            const double testDouble = 6.28;
            const String testString = "Hello, World!";
            const OSCColour testColour = { 10, 20, 150, 200 };
            const bool testTrue = true;
            const bool testFalse = false;

            const uint8 testBlobData[5] = { 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
            const MemoryBlock testBlob (testBlobData,  sizeof (testBlobData));

            msg.addInt32 (testInt);
            msg.addInt64 (testInt64);
            msg.addFloat32 (testFloat);
            msg.addDouble (testDouble);
            msg.addString (testString);
            msg.addBlob (testBlob);
            msg.addColour (testColour);
            msg.addNil ();
            msg.addImpulse ();
            msg.addBool (testTrue);
            msg.addBool (testFalse);


            expectEquals (msg.size(), numTestArgs);

            expectEquals (msg[0].getType(), OSCTypes::int32);
            expectEquals (msg[1].getType(), OSCTypes::int64);
            expectEquals (msg[2].getType(), OSCTypes::float32);
            expectEquals (msg[3].getType(), OSCTypes::double64);
            expectEquals (msg[4].getType(), OSCTypes::string);
            expectEquals (msg[5].getType(), OSCTypes::blob);
            expectEquals (msg[6].getType(), OSCTypes::colour);
            expectEquals (msg[7].getType(), OSCTypes::nil);
            expectEquals (msg[8].getType(), OSCTypes::impulse);
            expectEquals (msg[9].getType(), OSCTypes::T);
            expectEquals (msg[10].getType(), OSCTypes::F);


            expect (msg[0].isInt32());
            expect (msg[1].isInt64());
            expect (msg[2].isFloat32());
            expect (msg[3].isDouble());
            expect (msg[4].isString());
            expect (msg[5].isBlob());
            expect (msg[6].isColour());
            expect (msg[7].isNil());
            expect (msg[8].isImpulse());
            expect (msg[9].isBool());
            expect (msg[10].isBool());


            expectEquals (msg[0].getInt32(), testInt);
            expectEquals (msg[1].getInt64(), testInt64);
            expectEquals (msg[2].getFloat32(), testFloat);
            expectEquals (msg[3].getDouble(), testDouble);
            expectEquals (msg[4].getString(), testString);
            expect (msg[5].getBlob() == testBlob);
            expect (msg[6].getColour().toInt32() == testColour.toInt32());
            expectEquals (msg[9].getBool(), testTrue);
			expectEquals (msg[10].getBool(), testFalse);



            expect (msg.begin() + numTestArgs == msg.end());

            auto arg = msg.begin();
            expect (arg->isInt32());
            expectEquals (arg->getInt32(), testInt);
            ++arg;
            expect (arg->isInt64());
            expectEquals (arg->getInt64(), testInt64);
            ++arg;
            expect (arg->isFloat32());
            expectEquals (arg->getFloat32(), testFloat);
            ++arg;
            expect (arg->isDouble());
            expectEquals (arg->getDouble(), testDouble);
            ++arg;
            expect (arg->isString());
            expectEquals (arg->getString(), testString);
            ++arg;
            expect (arg->isBlob());
            expect (arg->getBlob() == testBlob);
            ++arg;
            expect (arg->isColour());
            expect (arg->getColour().toInt32() == testColour.toInt32());
            ++arg;
            expect (arg->isNil());
            ++arg;
            expect(arg->isImpulse());
            ++arg;
            expect(arg->isBool());
            expectEquals(arg->getBool(), testTrue);
            ++arg;
            expect(arg->isBool());
            expectEquals(arg->getBool(), testFalse);
            ++arg;

            expect (arg == msg.end());
        }


        beginTest ("Initialisation with argument list (C++11 only)");
        {
            int testInt = 42;
            float testFloat = 5.5;
            String testString = "Hello, World!";

            {
                OSCMessage msg ("/test", testInt);
                expect (msg.getAddressPattern().toString() == String ("/test"));
                expectEquals (msg.size(), 1);
                expect (msg[0].isInt32());
                expectEquals (msg[0].getInt32(), testInt);
            }
            {
                OSCMessage msg ("/test", testFloat);
                expect (msg.getAddressPattern().toString() == String ("/test"));
                expectEquals (msg.size(), 1);
                expect (msg[0].isFloat32());
                expectEquals (msg[0].getFloat32(), testFloat);
            }
            {
                OSCMessage msg ("/test", testString);
                expect (msg.getAddressPattern().toString() == String ("/test"));
                expectEquals (msg.size(), 1);
                expect (msg[0].isString());
                expectEquals (msg[0].getString(), testString);
            }
            {
                OSCMessage msg ("/test", testInt, testFloat, testString, testFloat, testInt);
                expect (msg.getAddressPattern().toString() == String ("/test"));
                expectEquals (msg.size(), 5);
                expect (msg[0].isInt32());
                expect (msg[1].isFloat32());
                expect (msg[2].isString());
                expect (msg[3].isFloat32());
                expect (msg[4].isInt32());

                expectEquals (msg[0].getInt32(), testInt);
                expectEquals (msg[1].getFloat32(), testFloat);
                expectEquals (msg[2].getString(), testString);
                expectEquals (msg[3].getFloat32(), testFloat);
                expectEquals (msg[4].getInt32(), testInt);
            }
        }
    }
};

static OSCMessageTests OSCMessageUnitTests;

#endif

} // namespace juce
