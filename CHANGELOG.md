# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Fixed 
- Initial structure to avoid loading of GUI elements in console mode - #2
- Exporter: Setting an invalid or empty file extension will no longer crash GTlab - #130
- BugFix: Small check in table class prevents crash on linux for tables with only one point
- Changed ObjectUI Standard Behaviour for renaming to allow spaces - #109
- GtVersionNumber: correct function for operator '>' and '<' -#159 
- GtProcessDock: Dummy process elements are no longer mutable - #162
- Examples search path was set to a specific 'examples' dir - #168
- Fixed a crash when zooming in on a graphics scene containing a dense grid - #179

### Changed
- Saving a project was moved to a separate thread - #127
- Changed the class structure for Datazones to make use of the externalization feature - #127
- Tasks can be skipped - #82, #176
- Exteded library refactoring - #161
- Removed "File->Import Project" menu. Instead, project are imported now via "File->Open Project". - #202
- Chrashed modules of former runs will not affect console application - #141

### Added
- Added dynamic interface to define and use arbitrary functions from other modules - #207
- Multiple labels can be selected and deleted at once - #111
- Integrated the hdf5 data format and added support to ex-/internalize datasets at runtime including the option to enable this feature on a project basis - #127
- Monitoring property for strings - #140
- Dark mode for GTlab - #163
- Shortcut settings for GTlab including the introduction of several new shortcuts and the option to maintain them in the preferences - #112, #171
- Versionnumber based on GtVersionNumber for tasks - #129
- Add some DataZone functions to Python interface - #177

### Removed
- Nozzle map was removed to be added in the map collection - #114 
- Removed DoubleMap container to be added to the post module - #116


## [1.7.2] - 2021-05-25
### Changed
- Update check functionality uses maintenance tool command line interface instead of parsing log files - #121
- Bug report url changed and enabled for non dev mode - #122

### Added
- Functionality to export framework footprint information  - #124
- Functionality to display framework footprint information in command line mode - #125

## [1.7.1] - 2021-05-17
### Fixed
- RPATH fix for all core libraries

## [1.7.0] - 2021-04-30
### Changed
- Modules are only excluded if the dependencies have a lower version number than required by the module

## [1.7.0-rc2] - 2020-12-21
### Added
- Functionality to exclude modules on framework startup  - #102

### Changed
- Framework version macro included in gtApp header

## [1.7.0-rc1] - 2020-11-18
### Added
- Ability to extend the object UI for existing classes  - #95
- Ability to write project specific comments  - #70
- Removing multiple selected projects from project explorer - #32
- Warning while closing GTlab if process is still running - #27
- GtResult: Convenience function to add result to label - #83

### Changed
- Module version information based on the framework versioning format (major, minor, patch)  - #89
- GtModeTypeProperty.getVal() is returning objectName instead of empty string - #34

### Fixed
- Removed inconsistency in handling of non-dimensional unit - #33

### Removed
- Process monitoring and dependency to qwt library - #9

## [1.7.0-alpha1] - 2020-11-06
### Added
- Convinience function to add datazones to resul-objects with labels easier  - #83
- ModeTypeProperty getVal() returns mode type id  - #34
- Improved comparison og ModeProperty and ModeTypeProperty - #34
- Option to remove multiple projects from session - #32

### Changed
- New general indication of version numbers. Used in modules and calculators, among others. - #89

### Fixed
- BugFix: Small check in table class prevents crash on linux for tables with only one point
- BugFix: Unitconversion for a non-dimensional unit

## [1.6.5] - 2020-09-16
### Fixed
- BugFix: Information about differences in version of the framework and project within ProjectAnalyzerDialog - #81
- BugFix: Invalidation of Access Data for Collections - #90
- BugFix: Wrong index in access data model when access data is removed - #92


## [1.6.4] - 2020-07-28
### Fixed
- BugFix: Fix crashes within the Process DockWidget if no selected project is open - #50
- BugFix: Potential memory leak in ProcessExecutor - #78
- BugFix: Auto delete application temp folder - #73
- BugFix: Locale options for linux builds - #80

### Added
- Handling of subproperties in connection editor - #39

## [1.6.3] - 2020-07-14
### Fixed
- BugFix: Invalid conversion from given type to variant - #77

## [1.6.2] - 2020-07-09
### Fixed
- BugFix: Deserialization of property lists of old type "string" - #76

## [1.6.1] - 2020-07-03
### Fixed
- BugFix: Potential memory leak in ProcessExecutor - #75
- BugFix: ContourPlot crash (Maps/AeroPlots) is fixed by several safty functions #74

## [1.6.0] - 2020-06-05
### Added
- ProjectAnalyzer to identify corrupted save data and framework incompatibilities - #65, #64, #62
- Objects of unknown classes are displayed within project/process tree - #1
- XML attributes within project files are saved in alphabetical order - #59
- Exporter and Importer for Processelements - #58
- Backup functionality for project files when the project is saved - #10
- Changelog file in repository - #72
- Installation steps in readme - #4

### Changed
- Migration to new build configuration (Qt12, MSVC 2017 Build Tools)

### Fixed 
- BugFix: Project Analyzer opens after the creation of a new project - #71
- BugFix: Process Importer does not change UUID if element already exist - #67
- BugFix: Renaming a project fails under certain conditions - #60
- BugFix: Linux Adaptations - #22
- BugFix: Skipping or unskipping process elements is not recorded in the redo/undo system - #15
- BugFix: 'Add...' button under 'Processes/Calculators' stays enabled after closing project - #13

### Removed
- Removed: Schedule functionality is moved to BasicTools Module - #63 
- Removed: Spline functionality is moved to BasicTools Module - #7 
- Removed: Filter functionality in the explorer widget to display results separately - #57
