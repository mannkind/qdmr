#ifndef OPENGD77BASE_CODEPLUG_HH
#define OPENGD77BASE_CODEPLUG_HH

#include "channel.hh"
#include "codeplug.hh"
#include "gpssystem.hh"
#include "contact.hh"
#include "zone.hh"

#include <QGeoCoordinate>


class OpenGD77BaseCodeplug : public Codeplug
{
  Q_OBJECT

public:
  /** Encodes an angle used to store locations. */
  static uint32_t encodeAngle(double degee);
  /** Decodes an angle used to store locations. */
  static double decodeAngle(uint32_t code);

  /** Encodes a selective call (tx/rx tone). */
  static uint16_t encodeSelectiveCall(const SelectiveCall &call);
  /** Decodes a selective call (tx/rx tone). */
  static SelectiveCall decodeSelectiveCall(uint16_t code);

public:
  /** Implements the base for all OpenGD77 channel encodings.
   */
  class ChannelElement: public Codeplug::Element
  {
  public:
    /** Possible channel types. */
    enum Mode {
      MODE_ANALOG = 0,   ///< Analog channel, aka FM.
      MODE_DIGITAL = 1   ///< Digital channel, aka DMR.
    };

    /** Alias to transmit. */
    enum class Alias {
      None, APRS, Text, Both
    };

    /** Possible squelch modes. */
    enum class SquelchMode {
      Global, Open, Normal, Closed
    };

  protected:
    /** Constructs a channel from the given memory. */
    ChannelElement(uint8_t *ptr, size_t size);

  public:
    /** Constructs a channel from the given memory. */
    explicit ChannelElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~ChannelElement();

    /** The size of the channel. */
    static constexpr unsigned int size() { return 0x0038; }

    /** Resets the channel. */
    virtual void clear();

    /** Returns the name of the channel. */
    virtual QString name() const;
    /** Sets the name of the channel. */
    virtual void setName(const QString &n);

    /** Returns the RX frequency of the channel. */
    virtual uint32_t rxFrequency() const;
    /** Sets the RX frequency of the channel. */
    virtual void setRXFrequency(uint32_t freq);
    /** Returns the TX frequency of the channel. */
    virtual uint32_t txFrequency() const;
    /** Sets the TX frequency of the channel. */
    virtual void setTXFrequency(uint32_t freq);

    /** Returns the channel mode. */
    virtual Mode mode() const;
    /** Sets the channel mode. */
    virtual void setMode(Mode mode);

    /** Returns @c true if the power-setting is global. */
    virtual bool globalPower() const;
    /** Returns the power setting of the channel. */
    virtual Channel::Power power() const;
    /** Sets the power setting of the channel. */
    virtual void setPower(Channel::Power pwr);
    /** Clears the power setting. The global power setting is used. */
    virtual void clearPower();

    /** Returns @c true, if a fixed position is set for the channel. */
    virtual bool hasFixedPosition() const;
    /** Returns the fixed position. */
    virtual QGeoCoordinate fixedPosition() const;
    /** Sets the fixed position for this channel. */
    virtual void setFixedPosition(const QGeoCoordinate &coordinate);
    /** Resets the fixed position. */
    virtual void clearFixedPosition();

    /** Returns the RX subtone. */
    virtual SelectiveCall rxTone() const;
    /** Sets the RX subtone. */
    virtual void setRXTone(const SelectiveCall &code);
    /** Returns the TX subtone. */
    virtual SelectiveCall txTone() const;
    /** Sets the TX subtone. */
    virtual void setTXTone(const SelectiveCall &code);

    /** Returns @c true if the channel is set to simplex. */
    virtual bool isSimplex() const;
    /** Sets the channel to simplex. */
    virtual void enableSimplex(bool enable);

    /** Returns @c true, if the power-save feature is enabled. */
    virtual bool powerSave() const;
    /** Enables/disables power-save. */
    virtual void enablePowerSave(bool enable);

    /** Returns @c true, if the "beep" is enabled. */
    virtual bool beep() const;
    /** Enables/disables "the beep". */
    virtual void enableBeep(bool enable);

    /** Returns @c true if the global DMR ID is overridden. */
    virtual bool hasDMRId() const;
    /** Retruns the DMR ID for this channel. */
    virtual unsigned int dmrId() const;
    /** Sets the DMR ID for this channel. */
    virtual void setDMRId(unsigned int id);
    /** Resets the DMR ID for this channel to the global one. */
    virtual void clearDMRId();

    /** Returns @c true if a group list is set. */
    virtual bool hasGroupList() const;
    /** Returns the group-list index. */
    virtual unsigned groupListIndex() const;
    /** Sets the group-list index. */
    virtual void setGroupListIndex(unsigned index);
    /** Clears the group list index. */
    virtual void clearGroupListIndex();

    /** Returns the color code. */
    virtual unsigned colorCode() const;
    /** Sets thecolor code. */
    virtual void setColorCode(unsigned cc);

    /** Returns @c true, if the APRS system index is set. */
    virtual bool hasAPRSIndex() const;
    /** Returns the APRS system index. */
    virtual unsigned int aprsIndex() const;
    /** Sets the APRS system index. */
    virtual void setAPRSIndex(unsigned int index);
    /** Resets the APRS system index. */
    virtual void clearAPRSIndex();

    /** Returns the alias transmitted on time slot 1. */
    virtual Alias aliasTimeSlot1() const;
    /** Sets the alias transmitted on time slot 1. */
    virtual void setAliasTimeSlot1(Alias alias);
    /** Returns the alias transmitted on time slot 2. */
    virtual Alias aliasTimeSlot2() const;
    /** Sets the alias transmitted on time slot 2. */
    virtual void setAliasTimeSlot2(Alias alias);

    /** Returns the time slot of the channel. */
    virtual DMRChannel::TimeSlot timeSlot() const;
    /** Sets the time slot of the channel. */
    virtual void setTimeSlot(DMRChannel::TimeSlot ts);

    /** Returns the bandwidth. */
    virtual FMChannel::Bandwidth bandwidth() const;
    /** Sets the bandwidth. */
    virtual void setBandwidth(FMChannel::Bandwidth bw);

    /** Returns @c true if RX only is enabled. */
    virtual bool rxOnly() const;
    /** Enables/disables RX only. */
    virtual void enableRXOnly(bool enable);

    /** Returns @c true if channel is skipped in a scan. */
    virtual bool skipScan() const;
    /** Enables/disables skipping in scan. */
    virtual void enableSkipScan(bool enable);
    /** Returns @c true if channel is skipped in zone scan. */
    virtual bool skipZoneScan() const;
    /** Enables/disables skipping in zone scan. */
    virtual void enableSkipZoneScan(bool enable);

    /** Returns @c true if VOX is enabled. */
    virtual bool vox() const;
    /** Enables/disables VOX. */
    virtual void enableVOX(bool enable);

    /** Returns the squelch mode*/
    virtual SquelchMode squelchMode() const;
    /** Retunrs the squelch level. */
    virtual unsigned int squelchLevel() const;
    /** Set the squelch level and mode. Level is ignored, if mode is not normal.*/
    virtual void setSquelch(SquelchMode mode, unsigned int level);

    /** Constructs a generic @c Channel object from the codeplug channel. */
    virtual Channel *toChannelObj(Context &ctx, const ErrorStack &err=ErrorStack()) const;
    /** Links a previously constructed channel to the rest of the configuration. */
    virtual bool linkChannelObj(Channel *c, Context &ctx, const ErrorStack &err=ErrorStack()) const;
    /** Initializes this codeplug channel from the given generic configuration. */
    virtual bool fromChannelObj(const Channel *c, Context &ctx, const ErrorStack &err=ErrorStack());

  public:
    /** Some limits for this element. */
    struct Limit {
      /** The maximum length of the name. */
      static constexpr unsigned int nameLength() { return 16; }
    };

  protected:
    /** Some internal offsets within the channel element. */
    struct Offset: public Element::Offset {
      /// @cond DO_NOT_DOCUMENT
      static constexpr unsigned int name() { return 0x0000; }
      static constexpr unsigned int rxFrequency() { return 0x0010; }
      static constexpr unsigned int txFrequency() { return 0x0014; }
      static constexpr unsigned int mode() { return 0x0018; }
      static constexpr unsigned int power() { return 0x0019; }
      static constexpr unsigned int latitude0() { return 0x001a; }
      static constexpr unsigned int latitude1() { return 0x001c; }
      static constexpr unsigned int latitude2() { return 0x001d; }
      static constexpr unsigned int longitude0() { return 0x001e; }
      static constexpr unsigned int longitude1() { return 0x001f; }
      static constexpr unsigned int rxTone() { return 0x0020; }
      static constexpr unsigned int txTone() { return 0x0022; }
      static constexpr unsigned int longitude2() { return 0x0024; }
      static constexpr Bit simplex() { return {0x0026, 3}; }
      static constexpr Bit useFixedLocation() { return {0x0026, 3}; }
      static constexpr Bit disablePowerSave() { return {0x0026, 5}; }
      static constexpr Bit disableBeep() { return {0x0026, 6}; }
      static constexpr Bit overrideDMRID() { return {0x0026, 7}; }
      static constexpr unsigned int dmrId() { return 0x0027; }
      static constexpr unsigned int groupList() { return 0x002b; }
      static constexpr unsigned int colorCode() { return 0x002c; }
      static constexpr unsigned int aprsIndex() { return 0x002d; }
      static constexpr Bit aliasTimeSlot2() { return { 0x030, 2}; }
      static constexpr Bit aliasTimeSlot1() { return { 0x030, 0}; }
      static constexpr Bit timeSlot() { return {0x0031, 6}; }
      static constexpr Bit bandwidth() { return {0x0033, 1}; }
      static constexpr Bit rxOnly() { return {0x0033, 2}; }
      static constexpr Bit skipScan() { return {0x0033, 4}; }
      static constexpr Bit skipZoneScan() { return {0x0033, 5}; }
      static constexpr Bit vox() { return {0x0033, 6}; }
      static constexpr unsigned int squelch() { return 0x0037; }
    };
  };


  /** Implements the base for channel banks in Radioddity codeplugs.
   *
   * Memory layout of a channel bank:
   * @verbinclude radioddity_channelbank.txt */
  class ChannelBankElement: public Element
  {
  protected:
    /** Hidden constructor. */
    ChannelBankElement(uint8_t *ptr, unsigned size);

  public:
    /** Constructor. */
    explicit ChannelBankElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~ChannelBankElement();

    /** The size of the channel bank. */
    static constexpr unsigned int size() { return 0x1c10; }

    /** Clears the bank. */
    void clear();

    /** Returns @c true if the channel is enabled. */
    virtual bool isEnabled(unsigned idx) const ;
    /** Enable/disable a channel in the bank. */
    virtual void enable(unsigned idx, bool enabled);
    /** Returns a pointer to the channel at the given index. */
    virtual uint8_t *get(unsigned idx) const;
    /** Retunrs the n-th channel. */
    ChannelElement channel(unsigned int n);

  public:
    /** Some limits for the channel bank. */
    struct Limit {
      /** The maximum number of channels. */
      static constexpr unsigned int channelCount() { return 128; }
    };

  protected:
    /** Some internal offset within the element. */
    struct Offset {
      /// @cond DO_NOT_DOCUMENT
      static constexpr unsigned int bitmask() { return 0x0000; }
      static constexpr unsigned int channels() { return 0x0010; }
      /// @endcond
    };
  };


  /** VFO Channel representation within the binary codeplug.
   *
   * Each channel requires 0x38b:
   * @verbinclude radioddity_vfochannel.txt */
  class VFOChannelElement: public ChannelElement
  {
  public:
    /** Possible offset frequency modes. */
    enum class OffsetMode {
      Off = 0,       ///< Disables transmit frequency offset.
      Positive = 1,  ///< Transmit offset frequency is positive (TX above RX).
      Negative = 2   ///< Transmit offset frequency is negative (TX below RX).
    };

    /** Possible tuning step sizes. */
    enum class StepSize {
      SS2_5kHz = 0,  ///< 2.5kHz
      SS5kHz = 1,    ///< 5kHz
      SS6_25kHz = 2, ///< 6.25kHz
      SS10kHz = 3,   ///< 10kHz
      SS12_5kHz = 4, ///< 12.5kHz
      SS20kHz = 5,   ///< 20kHz
      SS30kHz = 6,   ///< 30kHz
      SS50kHz = 7    ///< 50kHz
    };

  protected:
    /** Hidden constructor. */
    VFOChannelElement(uint8_t *ptr, unsigned size);

  public:
    /** Constructor. */
    explicit VFOChannelElement(uint8_t *ptr);

    void clear();

    /** The VFO channel has no name. */
    QString name() const;
    /** The VFO channel has no name. */
    void setName(const QString &name);

    /** Returns the tuning step-size in kHz. */
    virtual double stepSize() const;
    /** Sets the tuning step-size in kHz. */
    virtual void setStepSize(double kHz);

    /** Returns the transmit frequency offset mode. */
    virtual OffsetMode offsetMode() const;
    /** Returns the transmit frequency offset. */
    virtual double txOffset() const;
    /** Sets the transmit frequency offset in MHz. */
    virtual void setTXOffset(double f);
    /** Sets the transmit frequency offset mode. */
    virtual void setOffsetMode(OffsetMode mode);

  protected:
    /// @cond DO_NOT_DOCUMENT
    struct Offset: public ChannelElement::Offset {
      static constexpr Bit stepSize()                         { return {0x0036, 4} ; }
      static constexpr Bit offsetMode()                       { return {0x0036, 2} ; }
      static constexpr unsigned int txOffset()                { return 0x0034; }
    };
    /// @endcond
  };


  /** APRS system for OpenGD77 devices. */
  class APRSSettingsElement: public Element
  {
  public:
    enum class BaudRate {
      Baud300 = 1, Baud1200 = 0
    };

    enum class PositionPrecision {
      Max = 0,
      Mask1_8sec = 1,
      Mask3_6sec = 2,
      Mask18sec = 3,
      Mask36sec = 4,
      Mask3min = 5,
      Mask6min = 6,
      Mask30min = 7
    };

  public:
    /** Constructor from pointer. */
    explicit APRSSettingsElement(uint8_t *ptr);

  protected:
    /** Hidden constructor. */
    APRSSettingsElement(uint8_t *ptr, size_t size);

  public:
    /** The size of the channel bank. */
    static constexpr unsigned int size() { return 0x40; }

    /** Clears the bank. */
    void clear();

    /** Returns @c true, if the system is valid. */
    virtual bool isValid() const;

    /** Returns the name of the system. */
    virtual QString name() const;
    /** Sets the name of the system. */
    virtual void setName(const QString &name);

    /** Returns the source SSID. */
    virtual unsigned int sourceSSID() const;
    /** Sets the source SSID. */
    virtual void setSourceSSID(unsigned int ssid);

    /** Returns @c true, if a fixed position is send. */
    virtual bool hasFixedPosition() const;
    /** Returns the fixed position. */
    virtual QGeoCoordinate fixedPosition() const;
    /** Sets the fixed position. */
    virtual void setFixedPosition(const QGeoCoordinate &coor);
    /** Resets the fixed position. */
    virtual void clearFixedPosition();

    /** Retunrs the posiiton reporting precision. */
    virtual PositionPrecision positionPrecision() const;
    /** Sets the position reporting precision in degrees. */
    virtual void setPositionPrecision(PositionPrecision prec);

    /** Returns @c true, if the first via node is set. */
    virtual bool hasVia1() const;
    /** Returns the first via node call. */
    virtual QString via1Call() const;
    /** Returns the first via node ssid. */
    virtual unsigned int via1SSID() const;
    /** Sets the first via node. */
    virtual void setVia1(const QString &call, unsigned int ssid);
    /** Clears the first via node. */
    virtual void clearVia1();

    /** Returns @c true, if the second via node is set. */
    virtual bool hasVia2() const;
    /** Returns the second via node call. */
    virtual QString via2Call() const;
    /** Returns the second via node ssid. */
    virtual unsigned int via2SSID() const;
    /** Sets the second via node. */
    virtual void setVia2(const QString &call, unsigned int ssid);
    /** Clears the second via node. */
    virtual void clearVia2();

    /** Retunrs the icon. */
    virtual APRSSystem::Icon icon() const;
    /** Sets the icon. */
    virtual void setIcon(APRSSystem::Icon icon);

    /** Retunrs the comment text. */
    virtual QString comment() const;
    /** Sets the comment text. */
    virtual void setComment(const QString &text);

    /** Retunrs the baud-rate. */
    virtual BaudRate baudRate() const;
    /** Sets the baud rate. */
    virtual void setBaudRate(BaudRate rate);

    /** Encodes the APRS settings. */
    virtual bool encode(const APRSSystem *system, const Context &ctx, const ErrorStack &err=ErrorStack());
    /** Decodes some APRS settings. */
    virtual APRSSystem *decode(const Context &ctx, const ErrorStack &err=ErrorStack()) const;
    /** Links the ARPS settings. */
    virtual bool link(APRSSystem *system, const Context &ctx, const ErrorStack &err=ErrorStack());

  public:
    /** Some limits. */
    struct Limit: public Element::Limit {
      static constexpr unsigned int nameLength() { return 8; }
      static constexpr unsigned int commentLength() { return 23; }
    };

  protected:
    /// @cond DO_NOT_DOCUMENT
    struct Offset: public Element::Offset
    {
      static constexpr unsigned int name() { return 0x0000; }
      static constexpr unsigned int sourceSSID() { return 0x0008; }
      static constexpr unsigned int latitude() { return 0x0009; }
      static constexpr unsigned int longitude() { return 0x000c; }
      static constexpr unsigned int via1Call() { return 0x000f; }
      static constexpr unsigned int via1SSID() { return 0x0015; }
      static constexpr unsigned int via2Call() { return 0x0016; }
      static constexpr unsigned int via2SSID() { return 0x001c; }
      static constexpr unsigned int iconTable() { return 0x001d; }
      static constexpr unsigned int iconIndex() { return 0x001e; }
      static constexpr unsigned int comment() { return 0x001f; }
      static constexpr Bit positionPrecision() { return { 0x003d, 4}; }
      static constexpr Bit useFixedPosition() { return { 0x003d, 1}; }
      static constexpr Bit baudRate() { return { 0x003d, 0}; }
    };
    /// @endcond
  };


  /** APRS System bank. */
  class APRSSettingsBankElement: public Element
  {
  public:
    /** Constructor from pointer. */
    explicit APRSSettingsBankElement(uint8_t *ptr);

  protected:
    /** Hidden constructor. */
    APRSSettingsBankElement(uint8_t *ptr, size_t size);

  public:
    /** The size of the channel bank. */
    static constexpr unsigned int size() { return 0x40; }

    /** Clears the bank. */
    void clear();

    /** Retunrs the n-th APRS system. */
    APRSSettingsElement system(unsigned int idx) const;

  public:
    /** Some limits for the bank. */
    struct Limit: public Element::Limit {
      /** The total number of APRS systems. */
      static constexpr unsigned int systems() { return 8; }
    };

  public:
    /// @cond DO_NOT_DOCUMENT
    struct Offset: public Element::Offset {
      static constexpr unsigned int systems() { return 0x0000; }
      static constexpr unsigned int betweenSystems() { return APRSSettingsElement::size(); }
    };
    /// @endcond
  };


  /** DTMF contact element.
   * Just a name and DTMF number. */
  class DTMFContactElement: public Element
  {
  protected:
    /** Hidden constructor. */
    DTMFContactElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    explicit DTMFContactElement(uint8_t *ptr);

    /** Returns the size of the element. */
    static constexpr unsigned int size() { return 0x0020; }

    void clear();
    bool isValid() const;

    /** Returns the name. */
    virtual QString name() const;
    /** Sets the name. */
    virtual void setName(const QString &name);

    /** Returns the DTMF number. */
    virtual QString number() const;
    /** Sets the DTMF number. */
    virtual void setNumber(const QString &number);

    /** Encodes a number. */
    virtual bool encode(const DTMFContact *contact, const Context &ctx, const ErrorStack &err=ErrorStack());
    /** Decodes a number. */
    virtual DTMFContact *decode(const Context &ctx, const ErrorStack &err=ErrorStack());

  public:
    /** Some limits. Ü*/
    struct Limit: public Element::Limit {
      /** The maximum name length. */
      static constexpr unsigned int nameLength() { return 16; }
      /** The maximum length of the number. */
      static constexpr unsigned int numberLength() { return 16; }
    };

  protected:
    /// @cond DO_NOT_DOCUMENT
    struct Offset: public Element::Offset {
      static constexpr unsigned int name() { return 0x0000; }
      static constexpr unsigned int number() { return 0x0010; }
    };
    /// @endcond
  };


  /** DTMF contact bank. */
  class DTMFContactBankElement: public Element
  {
  protected:
    /** Hidden constructor. */
    DTMFContactBankElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    DTMFContactBankElement(uint8_t *ptr);

    /** Returns the size of the element. */
    static constexpr unsigned int size() { return Limit::contacts()*DTMFContactElement::size(); }

    void clear();

    /** Returns the n-th DTMF contact. */
    DTMFContactElement contact(unsigned int n) const;

  public:
    /** Some limits for the bank. */
    struct Limit: public Element::Limit {
      /** The total number of contacts. */
      static constexpr unsigned int contacts() { return 64; }
    };

  public:
    /// @cond DO_NOT_DOCUMENT
    struct Offset: public Element::Offset {
      static constexpr unsigned int contacts() { return 0x0000; }
      static constexpr unsigned int betweenContacts() { return DTMFContactElement::size(); }
    };
    /// @endcond
  };


  /** Implements the base class of boot settings for all OpenGD77 codeplugs. */
  class BootSettingsElement: public Element
  {
  protected:
    /** Hidden constructor. */
    BootSettingsElement(uint8_t *ptr, unsigned size);

  public:
    /** Constructor. */
    explicit BootSettingsElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~BootSettingsElement();

    /** Resets the settings. */
    void clear();

    /** Returns @c true if the text is shown on boot, other wise an image is shown. */
    virtual bool bootText() const;
    /** Enables/disables boot text. */
    virtual void enableBootText(bool enable);

    /** Returns @c true if the boot password is enabled. */
    virtual bool bootPasswordEnabled() const;
    /** Returns the boot password (6 digit). */
    virtual unsigned bootPassword() const;
    /** Sets the boot password (6 digit). */
    virtual void setBootPassword(unsigned passwd);
    /** Clear boot password. */
    virtual void clearBootPassword();

    /** Returns the first line. */
    virtual QString line1() const;
    /** Sets the first line. */
    virtual void setLine1(const QString &text);
    /** Returns the Second line. */
    virtual QString line2() const;
    /** Sets the second line. */
    virtual void setLine2(const QString &text);

    /** Encodes boot text settings from configuration. */
    virtual bool encode(const Context &ctx, const ErrorStack &err = ErrorStack());
    /** Updates the configuration with the boot text settings. */
    virtual bool decode(Context &ctx, const ErrorStack &err = ErrorStack());

  public:
    /** Some limits for the settings. */
    struct Limit: public Element::Limit {
      /** The total number of contacts. */
      static constexpr unsigned int lineLength() { return 16; }
    };

  public:
    /// @cond DO_NOT_DOCUMENT
    struct Offset: public Element::Offset {
      static constexpr unsigned int bootText() { return 0x0000; }
      static constexpr unsigned int bootPasswdEnable() { return 0x0001; }
      static constexpr unsigned int bootPasswd() { return 0x000c; }
      static constexpr unsigned int line1() { return 0x0028; }
      static constexpr unsigned int line2() { return 0x0038; }
    };
    /// @endcond
  };


  /** Represents a zone within OpenGD77 codeplugs. */
  class ZoneElement: public Element
  {
  protected:
    /** Hidden constructor. */
    ZoneElement(uint8_t *ptr, unsigned size);

  public:
    /** Constructor. */
    explicit ZoneElement(uint8_t *ptr);
    virtual ~ZoneElement();

    /** The size of the zone element. */
    static constexpr unsigned int size() { return 0x00b0; }

    /** Resets the zone. */
    void clear();
    /** Returns @c true if the zone is valid. */
    bool isValid() const;

    /** Returns the name of the zone. */
    virtual QString name() const;
    /** Sets the name of the zone. */
    virtual void setName(const QString &name);

    /** Returns @c true if a member is stored at the given index.
     * That is, if the index is not 0. */
    virtual bool hasMember(unsigned n) const;
    /** Returns the n-th member index (+1). */
    virtual unsigned member(unsigned n) const;
    /** Sets the n-th member index (+1). */
    virtual void setMember(unsigned n, unsigned idx);
    /** Clears the n-th member index. */
    virtual void clearMember(unsigned n);

    /** Resets this codeplug zone representation from the given generic @c Zone object. */
    virtual bool encode(const Zone *zone, Context &ctx, const ErrorStack &err=ErrorStack());
    /** Constructs a generic @c Zone object from this codeplug zone. */
    virtual Zone *decode(const Context &ctx, const ErrorStack &err=ErrorStack()) const;
    /** Links a previously constructed @c Zone object to the rest of the configuration. That is
     * linking to the referred channels. */
    virtual bool link(Zone *zone, Context &ctx, const ErrorStack &err=ErrorStack()) const;

  public:
    /** Some limits for zone elements. */
    struct Limit: public Element::Limit {
      /** The maximum length of the zone name. */
      static constexpr unsigned int nameLength() { return 16; }
      /** The maximum number of members. */
      static constexpr unsigned int memberCount() { return 80; }
    };

  protected:
    /** Some internal offsets within the element. */
    struct Offset: public Element::Offset {
      /// @cond DO_NOT_DOCUMENT
      static constexpr unsigned int name()  { return 0x0000; }
      static constexpr unsigned int channels() { return 0x0010; }
      static constexpr unsigned int betweenChannels() { return 0x0002; }
      /// @endcond
    };
  };


  /** Implements the base class for all zone banks of OpenGD77 codeplugs. */
  class ZoneBankElement: public Element
  {
  protected:
    /** Hidden constructor. */
    ZoneBankElement(uint8_t *ptr, unsigned size);

  public:
    /** Constructor. */
    explicit ZoneBankElement(uint8_t *ptr);
    /** Destructor. */
    ~ZoneBankElement();

    /** The size of the zone element. */
    static constexpr unsigned int size() {
      return 0x0020 + Limit::zoneCount()*ZoneElement::size();
    }

    /** Resets the bank. */
    void clear();

    /** Returns @c true if the zone is enabled. */
    virtual bool isEnabled(unsigned idx) const ;
    /** Enable/disable a zone in the bank. */
    virtual void enable(unsigned idx, bool enabled);

    /** Retunrs the n-th zone. */
    ZoneElement zone(unsigned int n);

  public:
    /** Some limits for the zone bank. */
    struct Limit: public Element::Limit {
      /** The maximum number of zones in this bank. */
      static constexpr unsigned int zoneCount() { return 68; }
    };

  protected:
    /** Some internal offsets within the element. */
    struct Offset: public Element::Offset {
      /// @cond DO_NOT_DOCUMENT
      static constexpr unsigned int bitmap() { return 0x0000; }
      static constexpr unsigned int zones()  { return 0x0020; }
      static constexpr unsigned int betweenZones() { return ZoneElement::size(); }
      /// @endcond
    };
  };


public:
  explicit OpenGD77BaseCodeplug(QObject *parent = nullptr);


};

#endif // OPENGD77BASE_CODEPLUG_HH