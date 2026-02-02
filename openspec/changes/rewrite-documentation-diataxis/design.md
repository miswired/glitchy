# Design: Documentation Rewrite

## Context

Glitchy is an open-source hardware hacking educational kit targeting security researchers, embedded engineers, and students. Documentation must serve:
- **Beginners**: No prior hardware/security knowledge assumed
- **Experts**: Quick access to technical details without wading through basics
- **Instructors**: Materials for live classroom teaching
- **Contributors**: Clear understanding of project architecture

The Diataxis framework (https://diataxis.fr/) provides a systematic approach by categorizing documentation into four types based on user needs.

## Goals / Non-Goals

### Goals
- Complete restructure of wiki following Diataxis principles
- Fill all documentation gaps (currently 3 major TODOs)
- Create reusable classroom lesson plans
- Design dark-mode compatible SVG diagrams
- Establish consistent formatting standards

### Non-Goals
- Video tutorials (future consideration)
- Translations/internationalization
- Interactive web-based labs
- Changes to firmware or web GUI code

## Decisions

### Decision: Diataxis Four-Quadrant Structure

**Rationale**: Diataxis is proven at Gatsby, Cloudflare, Django. It provides clear separation of concerns:

| Quadrant | User Need | Content Type |
|----------|-----------|--------------|
| Tutorial | Learning | Lessons with hands-on exercises |
| How-To | Doing | Task-focused guides |
| Reference | Looking up | Technical specifications |
| Explanation | Understanding | Conceptual background |

**Alternatives Considered**:
- Simple linear progression → Doesn't serve expert users well
- Topic-based organization → Mixes learning with reference
- Persona-based (beginner/advanced) → Creates duplication

### Decision: SVG Diagrams with CSS Variables

**Rationale**: SVGs scale perfectly, support theming, and are accessible.

```svg
<svg>
  <style>
    :root { --fg: currentColor; --bg: transparent; }
    @media (prefers-color-scheme: dark) {
      :root { --fg: #E0E0E0; --bg: #121212; }
    }
  </style>
</svg>
```

**Alternatives Considered**:
- PNG with light/dark versions → Doubles maintenance burden
- External Figma embeds → Requires account, breaks offline access
- ASCII art → Insufficient for circuit diagrams

### Decision: Wiki-Embedded Assets

**Rationale**: GitHub wiki repos support assets. Keeping diagrams with docs ensures:
- Single source of truth
- Version control for diagrams
- No external dependencies
- Works offline when cloned

**Directory Structure**:
```
glitchy.wiki/
├── assets/
│   └── diagrams/
│       ├── system-architecture.svg
│       ├── pinout-esp32s3.svg
│       ├── glitch-timing-waveform.svg
│       ├── power-analysis-flow.svg
│       ├── crowbar-circuit.svg
│       └── web-interface-map.svg
```

### Decision: Dual-Format Classroom Materials

**Rationale**: Users want both self-paced study and instructor-led classes.

Each lesson includes:
- **Instructor Guide**: Teaching notes, timing, discussion questions
- **Student Materials**: Exercises, expected outcomes
- **Slide Diagrams**: SVGs optimized for projection

**Lesson Structure**:
```markdown
# Lesson N: Topic

## Overview (Instructor)
- Duration: 45 minutes
- Prerequisites: Lesson N-1
- Materials needed: [list]

## Learning Objectives
Students will be able to:
- [ ] Objective 1
- [ ] Objective 2

## Lesson Content
### Section 1 (10 min)
[Content with instructor notes in blockquotes]

### Hands-On Exercise (20 min)
[Step-by-step with checkpoints]

### Discussion (10 min)
[Questions and wrap-up]

## Self-Study Notes
[Modifications for independent learners]
```

## Risks / Trade-offs

| Risk | Mitigation |
|------|------------|
| Large scope causes stalls | Phase implementation: research first, then core docs, then diagrams, then classroom |
| Diagrams become outdated | Store editable sources, document design system |
| Diataxis feels bureaucratic | Keep it pragmatic - serve users, not the framework |
| GitHub wiki limitations | Test rendering early, have fallback plans |
| Inaccurate technical content | Research-first approach with authoritative sources; dual-source verification |
| Unverifiable claims | Every technical claim must cite authoritative source; no unsourced assertions |
| AI hallucination in content | All generated content verified against primary sources before publication |

## Migration Plan

### Phase 0: Research & Verification (NEW - Critical)
1. Compile authoritative sources for all technical topics
2. Research glitching theory from academic papers
3. Research power analysis from foundational DPA/SPA literature
4. Verify circuit design against datasheets and application notes
5. Create Reference-Sources.md documenting all authoritative sources

**Authoritative Sources to Consult:**
- **Hardware Security**: CHES conference papers, Kocher et al. DPA papers, ChipWhisperer research
- **Electronics**: Espressif ESP32-S3 TRM, component datasheets, manufacturer app notes
- **Books**: "Power Analysis Attacks" (Mangard/Oswald/Popp), "The Hardware Hacker" (Huang)
- **Standards**: NIST guidelines on side-channel resistance

### Phase 1: Core Structure (Foundation)
1. Create new directory structure in wiki
2. Migrate existing content to appropriate quadrants
3. Add clear deprecation notices to old locations
4. Create Home.md navigation hub

### Phase 2: Fill Critical Gaps
1. Write PlatformIO setup guide
2. Write power analysis theory explanation
3. Write circuit explanation
4. Create troubleshooting guide

### Phase 3: Visual Assets
1. Design diagram style guide (colors, fonts, spacing)
2. Create system architecture diagram
3. Create pinout diagram
4. Create timing/waveform diagrams
5. Create power analysis flow diagram

### Phase 4: Reference Documentation
1. Document pin assignments
2. Document WebSocket API protocol
3. Document glitch parameters
4. Create Bill of Materials

### Phase 5: Classroom Materials
1. Create instructor overview guide
2. Create Lesson 1: Introduction to Hardware Security
3. Create Lesson 2: Setting Up Your Lab
4. Create Lesson 3: Power Glitching Hands-On
5. Create Lesson 4: Power Analysis Hands-On

### Rollback
- Keep original files until new structure is validated
- Use git branches for major changes
- Maintain redirects from old URLs

## Open Questions

1. **Lesson duration**: Should lessons target 45-min, 60-min, or 90-min class periods?
2. **Prerequisite knowledge**: Should we include embedded systems basics or link externally?
3. **Diagram tool**: Should we standardize on a specific SVG editor (Inkscape, Figma, etc.)?
4. **Assessment**: Should classroom lessons include quizzes or knowledge checks?
