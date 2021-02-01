/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2019 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#include "ndn-cxx/mgmt/nfd/face-status.hpp"
#include "ndn-cxx/encoding/block-helpers.hpp"
#include "ndn-cxx/encoding/encoding-buffer.hpp"
#include "ndn-cxx/encoding/tlv-nfd.hpp"
#include "ndn-cxx/util/concepts.hpp"
#include "ndn-cxx/util/string-helper.hpp"

namespace ndn {
namespace nfd {

BOOST_CONCEPT_ASSERT((StatusDatasetItem<FaceStatus>));

FaceStatus::FaceStatus()
  : m_nInInterests(0)
  , m_nInData(0)
  , m_nInNacks(0)
  , m_nOutInterests(0)
  , m_nOutData(0)
  , m_nOutNacks(0)
  , m_nInBytes(0)
  , m_nOutBytes(0)
  , m_interest_packet_size_min(0)
  , m_interest_packet_size_max(0)
  , m_interest_packet_size_avg(0.0)
  , m_interest_packet_size_std_dev(0.0)
  , m_data_packet_size_min(0)
  , m_data_packet_size_max(0)
  , m_data_packet_size_avg(0.0)
  , m_data_packet_size_std_dev(0.0)
  , m_interest_packet_components_min(0)
  , m_interest_packet_components_max(0)
  , m_interest_packet_components_avg(0.0)
  , m_interest_packet_components_std_dev(0.0)
  , m_data_packet_components_min(0)
  , m_data_packet_components_max(0)
  , m_data_packet_components_avg(0.0)
  , m_data_packet_components_std_dev(0.0)
{
}

FaceStatus::FaceStatus(const Block& block)
{
  this->wireDecode(block);
}

template<encoding::Tag TAG>
size_t
FaceStatus::wireEncode(EncodingImpl<TAG>& encoder) const
{
  size_t totalLength = 0;

  totalLength += prependStringBlock(encoder, tlv::nfd::DataComponentSizeStdDev, std::to_string(m_data_packet_components_std_dev));
  totalLength += prependStringBlock(encoder, tlv::nfd::DataComponentSizeAvg, std::to_string(m_data_packet_components_avg));
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::DataComponentSizeMax, m_data_packet_components_max);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::DataComponentSizeMin, m_data_packet_components_min);
  totalLength += prependStringBlock(encoder, tlv::nfd::InterestComponentSizeStdDev, std::to_string(m_interest_packet_components_std_dev));
  totalLength += prependStringBlock(encoder, tlv::nfd::InterestComponentSizeAvg, std::to_string(m_interest_packet_components_avg));
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::InterestComponentSizeMax, m_interest_packet_components_max);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::InterestComponentSizeMin, m_interest_packet_components_min);
  totalLength += prependStringBlock(encoder, tlv::nfd::DataPacketSizeStdDev, std::to_string(m_data_packet_size_std_dev));
  totalLength += prependStringBlock(encoder, tlv::nfd::DataPacketSizeAvg, std::to_string(m_data_packet_size_avg));
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::DataPacketSizeMax, m_data_packet_size_max);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::DataPacketSizeMin, m_data_packet_size_min);
  totalLength += prependStringBlock(encoder, tlv::nfd::InterestPacketSizeStdDev, std::to_string(m_interest_packet_size_std_dev));
  totalLength += prependStringBlock(encoder, tlv::nfd::InterestPacketSizeAvg, std::to_string(m_interest_packet_size_avg));
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::InterestPacketSizeMax, m_interest_packet_size_max);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::InterestPacketSizeMin, m_interest_packet_size_min);

  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::Flags, m_flags);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NOutBytes, m_nOutBytes);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NInBytes, m_nInBytes);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NOutNacks, m_nOutNacks);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NOutData, m_nOutData);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NOutInterests, m_nOutInterests);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NInNacks, m_nInNacks);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NInData, m_nInData);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NInInterests, m_nInInterests);
  if (m_mtu) {
    totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::Mtu, *m_mtu);
  }
  if (m_defaultCongestionThreshold) {
    totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::DefaultCongestionThreshold,
                                                  *m_defaultCongestionThreshold);
  }
  if (m_baseCongestionMarkingInterval) {
    totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::BaseCongestionMarkingInterval,
                                                  m_baseCongestionMarkingInterval->count());
  }
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::LinkType, m_linkType);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::FacePersistency, m_facePersistency);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::FaceScope, m_faceScope);
  if (m_expirationPeriod) {
    totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::ExpirationPeriod,
                                                  m_expirationPeriod->count());
  }
  totalLength += prependStringBlock(encoder, tlv::nfd::LocalUri, m_localUri);
  totalLength += prependStringBlock(encoder, tlv::nfd::Uri, m_remoteUri);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::FaceId, m_faceId);

  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(tlv::nfd::FaceStatus);
  return totalLength;
}

NDN_CXX_DEFINE_WIRE_ENCODE_INSTANTIATIONS(FaceStatus);

const Block&
FaceStatus::wireEncode() const
{
  if (m_wire.hasWire())
    return m_wire;

  EncodingEstimator estimator;
  size_t estimatedSize = wireEncode(estimator);

  EncodingBuffer buffer(estimatedSize, 0);
  wireEncode(buffer);

  m_wire = buffer.block();
  return m_wire;
}

void
FaceStatus::wireDecode(const Block& block)
{
  if (block.type() != tlv::nfd::FaceStatus) {
    NDN_THROW(Error("FaceStatus", block.type()));
  }

  m_wire = block;
  m_wire.parse();
  auto val = m_wire.elements_begin();

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::FaceId) {
    m_faceId = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required FaceId field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::Uri) {
    m_remoteUri = readString(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required Uri field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::LocalUri) {
    m_localUri = readString(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required LocalUri field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::ExpirationPeriod) {
    m_expirationPeriod.emplace(readNonNegativeInteger(*val));
    ++val;
  }
  else {
    m_expirationPeriod = nullopt;
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::FaceScope) {
    m_faceScope = readNonNegativeIntegerAs<FaceScope>(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required FaceScope field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::FacePersistency) {
    m_facePersistency = readNonNegativeIntegerAs<FacePersistency>(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required FacePersistency field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::LinkType) {
    m_linkType = readNonNegativeIntegerAs<LinkType>(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required LinkType field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::BaseCongestionMarkingInterval) {
    m_baseCongestionMarkingInterval.emplace(readNonNegativeInteger(*val));
    ++val;
  }
  else {
    m_baseCongestionMarkingInterval = nullopt;
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::DefaultCongestionThreshold) {
    m_defaultCongestionThreshold = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    m_defaultCongestionThreshold = nullopt;
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::Mtu) {
    m_mtu = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    m_mtu = nullopt;
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NInInterests) {
    m_nInInterests = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NInInterests field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NInData) {
    m_nInData = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NInData field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NInNacks) {
    m_nInNacks = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NInNacks field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NOutInterests) {
    m_nOutInterests = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NOutInterests field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NOutData) {
    m_nOutData = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NOutData field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NOutNacks) {
    m_nOutNacks = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NOutNacks field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NInBytes) {
    m_nInBytes = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NInBytes field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NOutBytes) {
    m_nOutBytes = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NOutBytes field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::Flags) {
    m_flags = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required Flags field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::InterestPacketSizeMin) {
    m_interest_packet_size_min = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required InterestPacketSizeMin field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::InterestPacketSizeMax) {
    m_interest_packet_size_max = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required InterestPacketSizeMax field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::InterestPacketSizeAvg) {
    m_interest_packet_size_avg = std::stof(readString(*val));
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required InterestPacketSizeAvg field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::InterestPacketSizeStdDev) {
    m_interest_packet_size_std_dev = std::stof(readString(*val));
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required InterestPacketSizeStdDev field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::DataPacketSizeMin) {
    m_data_packet_size_min = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required DataPacketSizeMin field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::DataPacketSizeMax) {
    m_data_packet_size_max = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required DataPacketSizeMax field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::DataPacketSizeAvg) {
    m_data_packet_size_avg = std::stof(readString(*val));
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required DataPacketSizeAvg field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::DataPacketSizeStdDev) {
    m_data_packet_size_std_dev = std::stof(readString(*val));
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required DataPacketSizeStdDev field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::InterestComponentSizeMin) {
    m_interest_packet_components_min = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required InterestComponentSizeMin field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::InterestComponentSizeMax) {
    m_interest_packet_components_max = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required InterestComponentSizeMax field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::InterestComponentSizeAvg) {
    m_interest_packet_components_avg = std::stof(readString(*val));
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required InterestComponentSizeAvg field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::InterestComponentSizeStdDev) {
    m_interest_packet_components_std_dev = std::stof(readString(*val));
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required InterestComponentSizeStdDev field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::DataComponentSizeMin) {
    m_data_packet_components_min = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required DataComponentSizeMin field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::DataComponentSizeMax) {
    m_data_packet_components_max = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required DataComponentSizeMax field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::DataComponentSizeAvg) {
    m_data_packet_components_avg = std::stof(readString(*val));
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required DataComponentSizeAvg field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::DataComponentSizeStdDev) {
    m_data_packet_components_std_dev = std::stof(readString(*val));
    ++val;
  }
  else {
    NDN_THROW(Error("Missing required DataComponentSizeStdDev field"));
  }
}

FaceStatus&
FaceStatus::setExpirationPeriod(time::milliseconds expirationPeriod)
{
  m_wire.reset();
  m_expirationPeriod = expirationPeriod;
  return *this;
}

FaceStatus&
FaceStatus::unsetExpirationPeriod()
{
  m_wire.reset();
  m_expirationPeriod = nullopt;
  return *this;
}

FaceStatus&
FaceStatus::setBaseCongestionMarkingInterval(time::nanoseconds interval)
{
  m_wire.reset();
  m_baseCongestionMarkingInterval = interval;
  return *this;
}

FaceStatus&
FaceStatus::unsetBaseCongestionMarkingInterval()
{
  m_wire.reset();
  m_baseCongestionMarkingInterval = nullopt;
  return *this;
}

FaceStatus&
FaceStatus::setDefaultCongestionThreshold(uint64_t threshold)
{
  m_wire.reset();
  m_defaultCongestionThreshold = threshold;
  return *this;
}

FaceStatus&
FaceStatus::unsetDefaultCongestionThreshold()
{
  m_wire.reset();
  m_defaultCongestionThreshold = nullopt;
  return *this;
}

FaceStatus&
FaceStatus::setMtu(uint64_t mtu)
{
  m_wire.reset();
  m_mtu = mtu;
  return *this;
}

FaceStatus&
FaceStatus::unsetMtu()
{
  m_wire.reset();
  m_mtu = nullopt;
  return *this;
}

FaceStatus&
FaceStatus::setNInInterests(uint64_t nInInterests)
{
  m_wire.reset();
  m_nInInterests = nInInterests;
  return *this;
}

FaceStatus&
FaceStatus::setNInData(uint64_t nInData)
{
  m_wire.reset();
  m_nInData = nInData;
  return *this;
}

FaceStatus&
FaceStatus::setNInNacks(uint64_t nInNacks)
{
  m_wire.reset();
  m_nInNacks = nInNacks;
  return *this;
}

FaceStatus&
FaceStatus::setNOutInterests(uint64_t nOutInterests)
{
  m_wire.reset();
  m_nOutInterests = nOutInterests;
  return *this;
}

FaceStatus&
FaceStatus::setNOutData(uint64_t nOutData)
{
  m_wire.reset();
  m_nOutData = nOutData;
  return *this;
}

FaceStatus&
FaceStatus::setNOutNacks(uint64_t nOutNacks)
{
  m_wire.reset();
  m_nOutNacks = nOutNacks;
  return *this;
}

FaceStatus&
FaceStatus::setNInBytes(uint64_t nInBytes)
{
  m_wire.reset();
  m_nInBytes = nInBytes;
  return *this;
}

FaceStatus&
FaceStatus::setNOutBytes(uint64_t nOutBytes)
{
  m_wire.reset();
  m_nOutBytes = nOutBytes;
  return *this;
}


FaceStatus&
FaceStatus::set_interest_packet_size_min(uint64_t interest_packet_size_min)
{
  m_wire.reset();
  m_interest_packet_size_min = interest_packet_size_min;
  return *this;
}

FaceStatus&
FaceStatus::set_interest_packet_size_max(uint64_t interest_packet_size_max)
{
  m_wire.reset();
  m_interest_packet_size_max = interest_packet_size_max;
  return *this;
}

FaceStatus&
FaceStatus::set_interest_packet_size_avg(float interest_packet_size_avg)
{
  m_wire.reset();
  m_interest_packet_size_avg = interest_packet_size_avg;
  return *this;
}

FaceStatus&
FaceStatus::set_interest_packet_size_std_dev(float interest_packet_size_std_dev)
{
  m_wire.reset();
  m_interest_packet_size_std_dev = interest_packet_size_std_dev;
  return *this;
}

FaceStatus&
FaceStatus::set_data_packet_size_min(uint64_t data_packet_size_min)
{
  m_wire.reset();
  m_data_packet_size_min = data_packet_size_min;
  return *this;
}

FaceStatus&
FaceStatus::set_data_packet_size_max(uint64_t data_packet_size_max)
{
  m_wire.reset();
  m_data_packet_size_max = data_packet_size_max;
  return *this;
}

FaceStatus&
FaceStatus::set_data_packet_size_avg(float data_packet_size_avg)
{
  m_wire.reset();
  m_data_packet_size_avg = data_packet_size_avg;
  return *this;
}

FaceStatus&
FaceStatus::set_data_packet_size_std_dev(float data_packet_size_std_dev)
{
  m_wire.reset();
  m_data_packet_size_std_dev = data_packet_size_std_dev;
  return *this;
}

FaceStatus&
FaceStatus::set_interest_packet_components_min(uint64_t interest_packet_components_min)
{
  m_wire.reset();
  m_interest_packet_components_min = interest_packet_components_min;
  return *this;
}

FaceStatus&
FaceStatus::set_interest_packet_components_max(uint64_t interest_packet_components_max)
{
  m_wire.reset();
  m_interest_packet_components_max = interest_packet_components_max;
  return *this;
}

FaceStatus&
FaceStatus::set_interest_packet_components_avg(float interest_packet_components_avg)
{
  m_wire.reset();
  m_interest_packet_components_avg = interest_packet_components_avg;
  return *this;
}

FaceStatus&
FaceStatus::set_interest_packet_components_std_dev(float interest_packet_components_std_dev)
{
  m_wire.reset();
  m_interest_packet_components_std_dev = interest_packet_components_std_dev;
  return *this;
}

FaceStatus&
FaceStatus::set_data_packet_components_min(uint64_t data_packet_components_min)
{
  m_wire.reset();
  m_data_packet_components_min = data_packet_components_min;
  return *this;
}

FaceStatus&
FaceStatus::set_data_packet_components_max(uint64_t data_packet_components_max)
{
  m_wire.reset();
  m_data_packet_components_max = data_packet_components_max;
  return *this;
}

FaceStatus&
FaceStatus::set_data_packet_components_avg(float data_packet_components_avg)
{
  m_wire.reset();
  m_data_packet_components_avg = data_packet_components_avg;
  return *this;
}

FaceStatus&
FaceStatus::set_data_packet_components_std_dev(float data_packet_components_std_dev)
{
  m_wire.reset();
  m_data_packet_components_std_dev = data_packet_components_std_dev;
  return *this;
}

bool
operator==(const FaceStatus& a, const FaceStatus& b)
{
  return a.getFaceId() == b.getFaceId() &&
      a.getRemoteUri() == b.getRemoteUri() &&
      a.getLocalUri() == b.getLocalUri() &&
      a.getFaceScope() == b.getFaceScope() &&
      a.getFacePersistency() == b.getFacePersistency() &&
      a.getLinkType() == b.getLinkType() &&
      a.getFlags() == b.getFlags() &&
      a.hasExpirationPeriod() == b.hasExpirationPeriod() &&
      (!a.hasExpirationPeriod() || a.getExpirationPeriod() == b.getExpirationPeriod()) &&
      a.hasBaseCongestionMarkingInterval() == b.hasBaseCongestionMarkingInterval() &&
      (!a.hasBaseCongestionMarkingInterval() ||
       a.getBaseCongestionMarkingInterval() == b.getBaseCongestionMarkingInterval()) &&
      a.hasDefaultCongestionThreshold() == b.hasDefaultCongestionThreshold() &&
      (!a.hasDefaultCongestionThreshold() ||
       a.getDefaultCongestionThreshold() == b.getDefaultCongestionThreshold()) &&
      a.hasMtu() == b.hasMtu() &&
      (!a.hasMtu() || a.getMtu() == b.getMtu()) &&
      a.getNInInterests() == b.getNInInterests() &&
      a.getNInData() == b.getNInData() &&
      a.getNInNacks() == b.getNInNacks() &&
      a.getNOutInterests() == b.getNOutInterests() &&
      a.getNOutData() == b.getNOutData() &&
      a.getNOutNacks() == b.getNOutNacks() &&
      a.getNInBytes() == b.getNInBytes() &&
      a.getNOutBytes() == b.getNOutBytes() &&
      a.get_interest_packet_size_min() == b.get_interest_packet_size_min() &&
      a.get_interest_packet_size_max() == b.get_interest_packet_size_max() &&
      a.get_interest_packet_size_avg() == b.get_interest_packet_size_avg() &&
      a.get_interest_packet_size_std_dev() == b.get_interest_packet_size_std_dev() &&
      a.get_data_packet_size_min() == b.get_data_packet_size_min() &&
      a.get_data_packet_size_max() == b.get_data_packet_size_max() &&
      a.get_data_packet_size_avg() == b.get_data_packet_size_avg() &&
      a.get_data_packet_size_std_dev() == b.get_data_packet_size_std_dev() &&
      a.get_interest_packet_components_min() == b.get_interest_packet_components_min() &&
      a.get_interest_packet_components_max() == b.get_interest_packet_components_max() &&
      a.get_interest_packet_components_avg() == b.get_interest_packet_components_avg() &&
      a.get_interest_packet_components_std_dev() == b.get_interest_packet_components_std_dev() &&
      a.get_data_packet_components_min() == b.get_data_packet_components_min() &&
      a.get_data_packet_components_max() == b.get_data_packet_components_max() &&
      a.get_data_packet_components_avg() == b.get_data_packet_components_avg() &&
      a.get_data_packet_components_std_dev() == b.get_data_packet_components_std_dev();
}

std::ostream&
operator<<(std::ostream& os, const FaceStatus& status)
{
  os << "Face(FaceId: " << status.getFaceId() << ",\n"
     << "     RemoteUri: " << status.getRemoteUri() << ",\n"
     << "     LocalUri: " << status.getLocalUri() << ",\n";

  if (status.hasExpirationPeriod()) {
    os << "     ExpirationPeriod: " << status.getExpirationPeriod() << ",\n";
  }
  else {
    os << "     ExpirationPeriod: infinite,\n";
  }

  os << "     FaceScope: " << status.getFaceScope() << ",\n"
     << "     FacePersistency: " << status.getFacePersistency() << ",\n"
     << "     LinkType: " << status.getLinkType() << ",\n";

  if (status.hasBaseCongestionMarkingInterval()) {
    os << "     BaseCongestionMarkingInterval: " << status.getBaseCongestionMarkingInterval() << ",\n";
  }

  if (status.hasDefaultCongestionThreshold()) {
    os << "     DefaultCongestionThreshold: " << status.getDefaultCongestionThreshold() << " bytes,\n";
  }

  if (status.hasMtu()) {
    os << "     Mtu: " << status.getMtu() << " bytes,\n";
  }

  os << "     Flags: " << AsHex{status.getFlags()} << ",\n"
     << "     Counters: {Interests: {in: " << status.getNInInterests() << ", "
     << "out: " << status.getNOutInterests() << "},\n"
     << "                Data: {in: " << status.getNInData() << ", "
     << "out: " << status.getNOutData() << "},\n"
     << "                Nacks: {in: " << status.getNInNacks() << ", "
     << "out: " << status.getNOutNacks() << "},\n"
     << "                bytes: {in: " << status.getNInBytes() << ", "
     << "out: " << status.getNOutBytes() << "}},\n"
     << "     Size: {Interests: {"
     << "min: " << status.get_interest_packet_size_min() << ", "
     << "max: " << status.get_interest_packet_size_max() << ", "
     << "avg: " << status.get_interest_packet_size_avg() << ", "
     << "dev: " << status.get_interest_packet_size_std_dev() << "},\n"
     << "            Data: {"
     << "min: " << status.get_data_packet_size_min() << ", "
     << "max: " << status.get_data_packet_size_max() << ", "
     << "avg: " << status.get_data_packet_size_avg() << ", "
     << "dev: " << status.get_data_packet_size_std_dev() << "}},\n"
     << "     Components: {Interests: {"
     << "min: " << status.get_interest_packet_components_min() << ", "
     << "max: " << status.get_interest_packet_components_max() << ", "
     << "avg: " << status.get_interest_packet_components_avg() << ", "
     << "dev: " << status.get_interest_packet_components_std_dev() << "},\n"
     << "                  Data: {"
     << "min: " << status.get_data_packet_components_min() << ", "
     << "max: " << status.get_data_packet_components_max() << ", "
     << "avg: " << status.get_data_packet_components_avg() << ", "
     << "dev: " << status.get_data_packet_components_std_dev() << "}}\n";

  return os << "     )";
}

} // namespace nfd
} // namespace ndn
