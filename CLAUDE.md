<!-- OPENSPEC:START -->
# OpenSpec Instructions

These instructions are for AI assistants working in this project.

Always open `@/openspec/AGENTS.md` when the request:
- Mentions planning or proposals (words like proposal, spec, change, plan)
- Introduces new capabilities, breaking changes, architecture shifts, or big performance/security work
- Sounds ambiguous and you need the authoritative spec before coding

Use `@/openspec/AGENTS.md` to learn:
- How to create and apply change proposals
- Spec format and conventions
- Project structure and guidelines

Keep this managed block so 'openspec update' can refresh the instructions.

<!-- OPENSPEC:END -->

---

# Current Project Status

**Last Updated**: 2026-02-01
**Active Work**: Documentation verification for live wiki content

## Active OpenSpec Change

**Change ID**: `rewrite-documentation-diataxis`
- **Location**: `openspec/changes/rewrite-documentation-diataxis/`
- **Status**: In Progress (9/78 tasks complete)
- **Current Phase**: Phase 0a - Verification of Live Documentation

### What's Been Completed

**Infrastructure** (Phase 0a.1 - COMPLETE):
- ✅ OpenSpec spec: `openspec/specs/documentation-verification/spec.md`
- ✅ Verification folder: `../glitchy.wiki/verifications/`
- ✅ Implementation guide: `../glitchy.wiki/verifications/README.md`
- ✅ Template file: `../glitchy.wiki/verifications/TEMPLATE.md`
- ✅ Agent instructions: Updated in `AGENTS.md`

**Live Documentation** (Published on wiki master branch):
- ✅ Home.md (updated with Diataxis navigation)
- ✅ Tutorial-Getting-Started.md
- ✅ Tutorial-First-Glitch.md
- ✅ HowTo-Setup-PlatformIO.md
- ✅ HowTo-Flash-Firmware.md
- ✅ HowTo-Configure-WiFi.md
- ✅ HowTo-Update-WebGUI.md
- ✅ HowTo-Setup-KiCad.md
- ✅ HowTo-Troubleshoot.md

**Additional Documentation** (On `new-documentation` branch, awaiting verification):
- Reference docs (10+ files)
- Explanation docs (5 files)
- Classroom materials (4 lessons)
- Diagrams (6 SVGs with PNG versions)
- Quick reference cards (4 files)
- Contributing, Safety-Guide, Style-Guide

### What's Next

**Immediate Priority** (Phase 0a.2-0a.9):
Verify technical accuracy of 8 live documents against authoritative sources:

1. **Next**: Tutorial-Getting-Started.md
2. Tutorial-First-Glitch.md
3. HowTo-Setup-PlatformIO.md
4. HowTo-Flash-Firmware.md
5. HowTo-Configure-WiFi.md
6. HowTo-Update-WebGUI.md
7. HowTo-Setup-KiCad.md
8. HowTo-Troubleshoot.md

**Verification Priority Order**: User journey (Getting-Started → First-Glitch → HowTos)

**After Verification**:
- Review and publish remaining docs from `new-documentation` branch
- Complete Phase 0 research for new documentation
- Create new content (tutorials, reference, explanation, classroom)

## Documentation Verification System

### Overview

All Glitchy documentation undergoes verification to ensure technical accuracy. This system was established 2026-02-01 to validate claims against authoritative sources.

### Quick Start for Agents

**When working on documentation**:
1. Read: `openspec/specs/documentation-verification/spec.md` (requirements)
2. Read: `../glitchy.wiki/verifications/README.md` (implementation guide)
3. Read: `AGENTS.md` (verification workflow and examples)
4. Use: `../glitchy.wiki/verifications/TEMPLATE.md` to create verification files

**Verification workflow**:
- Research authoritative sources BEFORE writing
- Document sources in verification files
- Mark confidence levels (HIGH/MEDIUM/LOW/UNKNOWN)
- Flag hardware-dependent claims for testing
- Update verification index and OpenSpec tasks

### Authoritative Sources

**PRIMARY** (preferred):
- ESP32-S3 TRM/Datasheet (Espressif official)
- Project source code with line numbers
- KiCad schematics
- Hardware testing results

**SECONDARY** (when primary unavailable):
- Academic papers (CHES, IEEE S&P, USENIX Security)
- Technical books ("Power Analysis Attacks", etc.)
- Framework documentation (PlatformIO, Arduino)

**INVALID** (do not use):
- Blog posts, forums, AI-generated content without verification

### Integration Points

- **OpenSpec tasks**: `openspec/changes/rewrite-documentation-diataxis/tasks.md`
- **Verification files**: `../glitchy.wiki/verifications/VERIFICATION-*.md`
- **Verification index**: `../glitchy.wiki/verifications/README.md`
- **Agent workflow**: `AGENTS.md` (detailed verification process)

## Related Repositories

- **Main repo**: `glitchy/` (firmware, PCB, OpenSpec)
- **Wiki repo**: `glitchy.wiki/` (documentation, verification files)
- **Web GUI**: `glitchy-vue-app/` (separate repo by Philip Kopyscinski)

## Git Branch Strategy

### glitchy.wiki Branches

- **master**: Live wiki content (8 documents currently published)
- **new-documentation**: Complete Diataxis rewrite (awaiting verification and staged deployment)

### Deployment Process

1. Verify document accuracy (create verification file)
2. Review with user
3. Approve corrections
4. Cherry-pick to master or merge approved content
5. Mark OpenSpec task complete

## Important Notes for Agents

- **Never modify live docs without user review**
- **Always verify technical claims against authoritative sources**
- **Mark hardware-dependent claims for testing** (WiFi, IPs, timing, voltages)
- **Use verification files to track sources and confidence**
- **Update tracking docs** (verification index, OpenSpec tasks)

## Questions or Issues?

- **Process**: See `AGENTS.md` or `openspec/specs/documentation-verification/spec.md`
- **Status**: See `openspec/changes/rewrite-documentation-diataxis/tasks.md`
- **Verification**: See `../glitchy.wiki/verifications/README.md`