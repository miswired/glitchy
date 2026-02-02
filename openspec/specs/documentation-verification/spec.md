# Capability: Documentation Verification

## Overview

The documentation verification system ensures all technical claims in Glitchy documentation are validated against authoritative sources. This prevents inaccurate information from reaching users and maintains documentation quality standards.

## Requirements

### Requirement: Verification File Per Document

Documentation verification SHALL create one verification file per source document being verified.

**Rationale**: Modular verification files allow parallel work, clear tracking, and isolated git history per document.

#### Scenario: Creating verification file for tutorial

- **GIVEN** a documentation file `Tutorial-Getting-Started.md` exists in the wiki
- **WHEN** verification is needed
- **THEN** create `verifications/VERIFICATION-Tutorial-Getting-Started.md`
- **AND** the verification filename SHALL match the source document filename exactly with `VERIFICATION-` prefix

#### Scenario: Verification file location

- **GIVEN** wiki documentation exists at repository root
- **WHEN** creating verification files
- **THEN** all verification files SHALL be stored in `verifications/` directory at repository root
- **AND** verification files SHALL NOT be nested in subdirectories

### Requirement: Verification File Format

Verification files SHALL include status summary, claims listing, authoritative sources, verification status, and notes for each claim.

**Rationale**: Standardized format ensures consistency and makes verification results easy to review.

#### Scenario: Recording verified claim

- **GIVEN** a technical claim "Pin 15 controls power glitch FET"
- **WHEN** verified against source code at `pins.h:15`
- **THEN** verification entry SHALL include:
  - Claim text
  - Source reference with file path and line number
  - Confidence level (HIGH/MEDIUM/LOW/UNKNOWN)
  - Status icon (✓ Verified | ❌ Incorrect | ⚠️ Needs Testing | ? Unknown)
  - Notes with additional context

#### Scenario: Recording unverified claim

- **GIVEN** a claim "Default WiFi SSID is 'Glitchy'"
- **WHEN** no source found in code or documentation
- **THEN** mark with status "⚠️ Needs Testing"
- **AND** set confidence "LOW" or "UNKNOWN"
- **AND** note what testing is required

#### Scenario: Status summary header

- **GIVEN** a verification file with multiple claims
- **WHEN** updating verification status
- **THEN** file SHALL include header with:
  - Overall status (In Progress / Complete / Needs Review)
  - Count of verified claims (e.g., "5/12 verified")
  - Last updated date
  - Link to source document being verified

### Requirement: Confidence Levels

All verified claims SHALL be marked with a confidence level indicating the strength of evidence.

**Rationale**: Distinguishes between rock-solid evidence (official docs) and assumptions that need testing.

#### Scenario: HIGH confidence assignment

- **GIVEN** a claim verified against official documentation, source code, or hardware testing
- **WHEN** recording verification status
- **THEN** set confidence to "HIGH"
- **EXAMPLES**:
  - ESP32-S3 Technical Reference Manual
  - Verified in project source code
  - Tested on physical Glitchy hardware

#### Scenario: MEDIUM confidence assignment

- **GIVEN** a claim inferred from verified sources or found in secondary sources
- **WHEN** recording verification status
- **THEN** set confidence to "MEDIUM"
- **EXAMPLES**:
  - Logically derived from HIGH confidence claims
  - Academic papers or technical books
  - Component datasheets for non-primary components

#### Scenario: LOW confidence assignment

- **GIVEN** a claim based on assumptions, anecdotal evidence, or incomplete sources
- **WHEN** recording verification status
- **THEN** set confidence to "LOW"
- **EXAMPLES**:
  - Assumed from project naming
  - Blog posts or forum discussions
  - Untested configurations

#### Scenario: UNKNOWN confidence assignment

- **GIVEN** a claim with no source found
- **WHEN** recording verification status
- **THEN** set confidence to "UNKNOWN"
- **AND** note that investigation or expert review is required

### Requirement: Authoritative Sources

Technical claims SHALL be verified against authoritative sources only.

**Rationale**: Ensures accuracy by requiring primary sources rather than secondary interpretations or assumptions.

#### Scenario: PRIMARY source - Official documentation

- **GIVEN** a claim about ESP32-S3 hardware capabilities
- **WHEN** verifying the claim
- **THEN** acceptable sources include:
  - Espressif ESP32-S3 Technical Reference Manual
  - ESP32-S3 Datasheet
  - Official Espressif documentation

#### Scenario: PRIMARY source - Project source code

- **GIVEN** a claim about Glitchy firmware behavior
- **WHEN** verifying the claim
- **THEN** verify against actual source code
- **AND** record file path and line numbers
- **EXAMPLE**: `Code/Glitchy/include/pins.h:15`

#### Scenario: PRIMARY source - Hardware schematics

- **GIVEN** a claim about circuit design or pin connections
- **WHEN** verifying the claim
- **THEN** verify against KiCad schematic files
- **AND** record schematic reference and revision

#### Scenario: SECONDARY source - Academic papers

- **GIVEN** a claim about glitching or power analysis theory
- **WHEN** official documentation unavailable
- **THEN** acceptable secondary sources include:
  - Peer-reviewed academic papers (CHES, IEEE S&P, USENIX Security)
  - Technical books (e.g., "Power Analysis Attacks" by Mangard et al.)
- **AND** record full citation

#### Scenario: INVALID source - Unverifiable

- **GIVEN** a claim found only in blog posts, forums, or undocumented code comments
- **WHEN** no authoritative source available
- **THEN** mark as "UNVERIFIED"
- **AND** note "Requires expert review" or "Needs hardware testing"

#### Scenario: Hardware testing as source

- **GIVEN** a claim about runtime behavior (WiFi SSID, default IP, timing values)
- **WHEN** not documented in code or configuration
- **THEN** verification requires testing on physical Glitchy hardware
- **AND** mark as "⚠️ Needs Testing" until confirmed
- **AND** record test procedure and results when tested

### Requirement: Verification Index

The verification directory SHALL maintain a README.md index showing status of all verifications.

**Rationale**: Provides quick overview of verification progress and entry point for the verification system.

#### Scenario: Checking overall progress

- **GIVEN** multiple verification files exist in `verifications/` directory
- **WHEN** user wants to check verification status
- **THEN** `verifications/README.md` SHALL display table with:
  - Document name (linked to verification file)
  - Status (Not Started / In Progress / Complete / Needs Review)
  - Count of verified claims
  - Count of issues found
  - Last updated date

#### Scenario: Index includes implementation guide

- **GIVEN** a contributor wants to create a verification file
- **WHEN** they read `verifications/README.md`
- **THEN** README SHALL include:
  - Overview of verification system
  - How to create new verification files
  - Link to TEMPLATE.md
  - Verification checklist
  - Reference to OpenSpec spec (this document)

#### Scenario: Index links to OpenSpec tracking

- **GIVEN** verification work is tracked in OpenSpec
- **WHEN** reading verification README
- **THEN** include link to OpenSpec tasks.md for high-level tracking
- **EXAMPLE**: "See `../glitchy/openspec/changes/rewrite-documentation-diataxis/tasks.md` for task tracking"

### Requirement: Verification Template

A template file SHALL be provided to ensure consistent verification file format.

**Rationale**: Reduces friction for creating new verification files and ensures format consistency.

#### Scenario: Creating new verification from template

- **GIVEN** a new document needs verification
- **WHEN** contributor creates verification file
- **THEN** copy `verifications/TEMPLATE.md`
- **AND** rename to `VERIFICATION-{document-name}.md`
- **AND** fill in document-specific content following template structure

#### Scenario: Template includes examples

- **GIVEN** contributor is new to verification process
- **WHEN** using the template
- **THEN** template SHALL include:
  - Example claims with all confidence levels
  - Example status icons
  - Example source citations
  - Comments explaining each section

### Requirement: OpenSpec Integration

Documentation verification SHALL integrate with OpenSpec task tracking at a high level.

**Rationale**: OpenSpec tracks WHAT documents need verification, verification files track HOW claims were verified.

#### Scenario: Task completion tracking

- **GIVEN** a document verification is complete
- **WHEN** all claims verified and issues resolved
- **THEN** mark corresponding task in OpenSpec tasks.md as complete
- **AND** update verification file status to "Complete"
- **AND** update verification index

#### Scenario: High-level task structure

- **GIVEN** OpenSpec change for documentation verification
- **WHEN** defining tasks in tasks.md
- **THEN** create one task per document being verified
- **EXAMPLE**: "- [ ] 0.2 Verify Tutorial-Getting-Started.md"
- **AND** reference verification files for details

### Requirement: Agent Instructions

AI agents working on documentation SHALL be instructed to follow verification process.

**Rationale**: Ensures future documentation changes maintain quality standards.

#### Scenario: Agent modifies documentation

- **GIVEN** an AI agent creates or modifies documentation
- **WHEN** documentation contains technical claims
- **THEN** agent SHALL create or update verification file
- **AND** verify claims against authoritative sources
- **AND** mark claims appropriately (verified, needs testing, etc.)

#### Scenario: Agent instructions location

- **GIVEN** agent instructions exist in project
- **WHEN** documenting verification process
- **THEN** add verification workflow to:
  - `AGENTS.md` or `CLAUDE.md` in glitchy repository
  - Reference OpenSpec spec for detailed requirements
  - Link to `verifications/README.md` for implementation guide

## Non-Requirements

### Not Required: Automated Verification

The system does NOT require automated testing or CI/CD integration. Verification is a manual process performed by humans and AI agents during documentation work.

### Not Required: Version Tracking

The system does NOT require tracking verification across documentation versions. Git history provides version control. Re-verification is performed when documentation changes.

### Not Required: External Tool Integration

The system does NOT require integration with external documentation tools, linters, or validators beyond standard markdown and git.
