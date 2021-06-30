#include "d878uv.hh"
#include "config.hh"
#include "logger.hh"

#include "d878uv_codeplug.hh"
// uses same callsign db as 878
#include "d868uv_callsigndb.hh"

#define RBSIZE 16
#define WBSIZE 16


static Radio::Features _d878uv_features =
{
  // show beta-warning
  .betaWarning = true,

  // general capabilities
  .hasDigital  = true,
  .hasAnalog   = true,

  .frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.}, {220., 225.}, {400., 480.} },

  // general limits
  .maxRadioIDs        = 250,
  .maxNameLength      = 16,
  .maxIntroLineLength = 14,

  // channel limits
  .maxChannels                  = 4000,
  .maxChannelNameLength         = 16,
  .allowChannelNoDefaultContact = false,

  // zone limits
  .maxZones               = 250,
  .maxZoneNameLength      = 16,
  .maxChannelsInZone      = 250,
  .hasABZone              = false,

  // scanlist limits
  .hasScanlists           = true,
  .maxScanlists           = 250,
  .maxScanlistNameLength  = 16,
  .maxChannelsInScanlist  = 31,
  .scanListNeedsPriority  = false,

  // contact list limits
  .maxContacts            = 10000,
  .maxContactNameLength   = 16,

  // rx group list limits
  .maxGrouplists          = 250,
  .maxGrouplistNameLength = 16,
  .maxContactsInGrouplist = 64,

  .hasGPS = true,
  .maxGPSSystems = 8,

  .hasAPRS = true,
  .maxAPRSSystems = 1,

  .hasRoaming               = true,
  .maxRoamingChannels       = 250,
  .maxRoamingZones          = 64,
  .maxChannelsInRoamingZone = 64,

  // call-sign database limits
  .hasCallsignDB          = true,  // hasCallsignDB
  .callsignDBImplemented  = true,  // callsignDBImplemented
  .maxCallsignsInDB       = 200000 // maxCallsignsInDB
};


D878UV::D878UV(AnytoneInterface *device, QObject *parent)
  : AnytoneRadio("Anytone AT-D878UV", device, parent), _features(_d878uv_features)
{
  _codeplug = new D878UVCodeplug(this);
  _callsigns = new D868UVCallsignDB(this);

  // Get device info and determine supported TX frequency bands
  AnytoneInterface::RadioInfo info; _dev->getInfo(info);
  switch (info.bands) {
  case 0x00:
  case 0x01:
  case 0x04:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.}, {400., 480.} };
    break;
  case 0x02:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.}, {430., 440.} };
    break;
  case 0x03:
  case 0x05:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {144., 146.}, {430., 440.} };
    break;
  case 0x06:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.}, {446., 447.} };
    break;
  case 0x07:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {144., 148.}, {420., 450.} };
    break;
  case 0x08:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.}, {400., 470.} };
    break;
  case 0x09:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {144., 146.}, {430., 432.} };
    break;
  case 0x0a:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {144., 148.}, {430., 450.} };
    break;
  case 0x0b:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.}, {400., 520.} };
    break;
  case 0x0c:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.}, {400., 490.} };
    break;
  case 0x0d:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.}, {403., 470.} };
    break;
  case 0x0e:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.}, {220.,225.}, {400., 520.} };
    break;
  case 0x0f:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {144., 148.}, {400., 520.} };
    break;
  case 0x10:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {144., 147.}, {430., 440.} };
    break;
  case 0x11:
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.} };
    break;
  default:
    logInfo() << "Unknown band-code" << QString::number(int(info.bands), 16)
              << ": Set freq range to 136-174MHz and 400-480MHz.";
    _features.frequencyLimits = QVector<Radio::Features::FrequencyRange>{ {136., 174.}, {400., 480.} };
    break;
  }

  QStringList bands;
  foreach(Radio::Features::FrequencyRange r, _features.frequencyLimits.ranges) {
    bands.append(tr("%1-%2MHz").arg(r.min).arg(r.max));
  }
  logDebug() << "Got band-code " << QString::number(int(info.bands), 16)
             << ": Limit TX frequencies to " << bands.join(", ") << ".";
}

const Radio::Features &
D878UV::features() const {
  return _features;
}


VerifyIssue::Type
D878UV::verifyConfig(Config *config, QList<VerifyIssue> &issues, const VerifyFlags &flags) {
  QString supported = "V100";
  VerifyIssue::Type issue = AnytoneRadio::verifyConfig(config, issues, flags);
  if (supported < _version) {
    issues.append(VerifyIssue(
                    VerifyIssue::WARNING,
                    tr("You are likely running a newer firmware version (%1) than supported (%2) by qdmr. "
                       "Notify the developers of qdmr about the new firmware version.").arg(_version, supported)));
    issue = std::max(issue, VerifyIssue::WARNING);
  } else if (supported > _version) {
    issues.append(VerifyIssue(
                    VerifyIssue::WARNING,
                    tr("You are likely running an older firmware version (%1) than supported (%2) by qdmr. "
                       "Condsider updating your firmware.").arg(_version, supported)));
    issue = std::max(issue, VerifyIssue::WARNING);
  }
  return issue;
}
