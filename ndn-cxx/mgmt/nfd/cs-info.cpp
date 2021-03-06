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

#include "ndn-cxx/mgmt/nfd/cs-info.hpp"
#include "ndn-cxx/encoding/block-helpers.hpp"
#include "ndn-cxx/encoding/encoding-buffer.hpp"
#include "ndn-cxx/encoding/tlv-nfd.hpp"
#include "ndn-cxx/util/concepts.hpp"

#include <iostream>

namespace ndn {
namespace nfd {

BOOST_CONCEPT_ASSERT((StatusDatasetItem<CsInfo>));

CsInfo::CsInfo()
  : m_capacity(0)
  , m_nEntries(0)
  , m_nHits(0)
  , m_nMisses(0)
  , m_policyName("")
  , m_minSize(0)
  , m_maxSize(0)
  , m_averageSize(0.0)
  , m_stdDevSize(0.0)
  , m_validSignaturePackets(0)
  , m_invalidSignaturePackets(0)
{
}

CsInfo::CsInfo(const Block& block)
{
  this->wireDecode(block);
}

template<encoding::Tag TAG>
size_t
CsInfo::wireEncode(EncodingImpl<TAG>& encoder) const
{
  size_t totalLength = 0;

  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NMisses, m_nMisses);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NHits, m_nHits);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::NCsEntries, m_nEntries);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::Flags, m_flags.to_ullong());
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::Capacity, m_capacity);
  totalLength += prependStringBlock(encoder, tlv::nfd::PolicyName, m_policyName);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::MinSize, m_minSize);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::MaxSize, m_maxSize);
  totalLength += prependStringBlock(encoder, tlv::nfd::AverageSize, std::to_string(m_averageSize));
  totalLength += prependStringBlock(encoder, tlv::nfd::StdDevSize, std::to_string(m_stdDevSize));
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::ValidSignaturePackets, m_validSignaturePackets);
  totalLength += prependNonNegativeIntegerBlock(encoder, tlv::nfd::InvalidSignaturePackets, m_invalidSignaturePackets);

  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(tlv::nfd::CsInfo);
  return totalLength;
}

NDN_CXX_DEFINE_WIRE_ENCODE_INSTANTIATIONS(CsInfo);

const Block&
CsInfo::wireEncode() const
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
CsInfo::wireDecode(const Block& block)
{
  if (block.type() != tlv::nfd::CsInfo) {
    NDN_THROW(Error("CsInfo", block.type()));
  }
  m_wire = block;
  m_wire.parse();
  auto val = m_wire.elements_begin();

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::InvalidSignaturePackets) {
    m_invalidSignaturePackets = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required InvalidSignaturePackets field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::ValidSignaturePackets) {
    m_validSignaturePackets = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required ValidSignaturePackets field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::StdDevSize) {
    m_stdDevSize = std::stof(readString(*val));
    ++val;
  }
  else {
    NDN_THROW(Error("missing required StdDevSize field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::AverageSize) {
    m_averageSize = std::stof(readString(*val));
    ++val;
  }
  else {
    NDN_THROW(Error("missing required AverageSize field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::MaxSize) {
    m_maxSize = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required MaxSize field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::MinSize) {
    m_minSize = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required MinSize field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::PolicyName) {
    m_policyName = readString(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required PolicyName field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::Capacity) {
    m_capacity = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required Capacity field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::Flags) {
    m_flags = FlagsBitSet(static_cast<unsigned long long>(readNonNegativeInteger(*val)));
    ++val;
  }
  else {
    NDN_THROW(Error("missing required Flags field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NCsEntries) {
    m_nEntries = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NCsEntries field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NHits) {
    m_nHits = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NHits field"));
  }

  if (val != m_wire.elements_end() && val->type() == tlv::nfd::NMisses) {
    m_nMisses = readNonNegativeInteger(*val);
    ++val;
  }
  else {
    NDN_THROW(Error("missing required NMisses field"));
  }
}

CsInfo&
CsInfo::setCapacity(uint64_t capacity)
{
  m_wire.reset();
  m_capacity = capacity;
  return *this;
}

CsInfo&
CsInfo::setEnableAdmit(bool enableAdmit)
{
  m_wire.reset();
  m_flags[BIT_CS_ENABLE_ADMIT] = enableAdmit;
  return *this;
}

CsInfo&
CsInfo::setEnableServe(bool enableServe)
{
  m_wire.reset();
  m_flags[BIT_CS_ENABLE_SERVE] = enableServe;
  return *this;
}

CsInfo&
CsInfo::setNEntries(uint64_t nEntries)
{
  m_wire.reset();
  m_nEntries = nEntries;
  return *this;
}

CsInfo&
CsInfo::setNHits(uint64_t nHits)
{
  m_wire.reset();
  m_nHits = nHits;
  return *this;
}

CsInfo&
CsInfo::setNMisses(uint64_t nMisses)
{
  m_wire.reset();
  m_nMisses = nMisses;
  return *this;
}

CsInfo&
CsInfo::setPolicyName(const std::string& policyName)
{
  m_wire.reset();
  m_policyName = policyName;
  return *this;
}

CsInfo&
CsInfo::setMinSize(uint64_t minSize)
{
  m_wire.reset();
  m_minSize = minSize;
  return *this;
}

CsInfo&
CsInfo::setMaxSize(uint64_t maxSize)
{
  m_wire.reset();
  m_maxSize = maxSize;
  return *this;
}

CsInfo&
CsInfo::setAverageSize(float averageSize)
{
  m_wire.reset();
  m_averageSize = averageSize;
  return *this;
}

CsInfo&
CsInfo::setStdDevSize(float stdDevSize)
{
  m_wire.reset();
  m_stdDevSize = stdDevSize;
  return *this;
}

CsInfo&
CsInfo::setValidSignaturePackets(uint64_t validSignaturePackets)
{
  m_wire.reset();
  m_validSignaturePackets = validSignaturePackets;
  return *this;
}

CsInfo&
CsInfo::setInvalidSignaturePackets(uint64_t invalidSignaturePackets)
{
  m_wire.reset();
  m_invalidSignaturePackets = invalidSignaturePackets;
  return *this;
}

bool
operator==(const CsInfo& a, const CsInfo& b)
{
  return a.getCapacity() == b.getCapacity() &&
      a.getEnableAdmit() == b.getEnableAdmit() &&
      a.getEnableServe() == b.getEnableServe() &&
      a.getNEntries() == b.getNEntries() &&
      a.getNHits() == b.getNHits() &&
      a.getNMisses() == b.getNMisses() &&
      a.getPolicyName() == b.getPolicyName() &&
      a.getMinSize() == b.getMinSize() &&
      a.getMaxSize() == b.getMaxSize() &&
      a.getAverageSize() == b.getAverageSize() &&
      a.getStdDevSize() == b.getStdDevSize() &&
      a.getValidSignaturePackets() == b.getValidSignaturePackets() &&
      a.getInvalidSignaturePackets() == b.getInvalidSignaturePackets();
}

std::ostream&
operator<<(std::ostream& os, const CsInfo& csi)
{
  return os << "CsInfo: "
            << csi.getNEntries() << " entries, " << csi.getCapacity() << " max, "
            << (csi.getEnableAdmit() ? "admit enabled, " : "admit disabled, ")
            << (csi.getEnableServe() ? "serve enabled, " : "serve disabled, ")
            << csi.getNHits() << (csi.getNHits() == 1 ? " hit, " : " hits, ")
            << csi.getNMisses() << (csi.getNMisses() == 1 ? " miss" : " misses, ")
            << csi.getPolicyName() << " policy, " << csi.getMinSize() << " min entry size, "
            << csi.getMaxSize() << " max entry size, "
            << csi.getAverageSize() << " avg entry size, "
            << csi.getStdDevSize() << " entry size std dev, "
            << csi.getValidSignaturePackets() << " valid sig packets, "
            << csi.getInvalidSignaturePackets() << " invalid sig packets";
}

} // namespace nfd
} // namespace ndn
