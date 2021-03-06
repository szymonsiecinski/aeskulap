/*
 *
 *  Copyright (C) 1994-2005, OFFIS
 *
 *  This software and supporting documentation were developed by
 *
 *    Kuratorium OFFIS e.V.
 *    Healthcare Information and Communication Systems
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
 *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
 *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
 *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
 *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
 *
 *  Module:  dcmdata
 *
 *  Author:  Andrew Hewett
 *
 *  Purpose: a dictionary entry in the loadable DICOM data dictionary
 *
 *  Last Update:      $Author: braindead $
 *  Update Date:      $Date: 2007/04/24 09:53:25 $
 *  Source File:      $Source: /sources/aeskulap/aeskulap/dcmtk/dcmdata/libsrc/dcdicent.cc,v $
 *  CVS/RCS Revision: $Revision: 1.2 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#define INCLUDE_CSTDIO
#define INCLUDE_CSTRING
#include "dcmtk/ofstd/ofstdinc.h"

#include "dcmtk/dcmdata/dcdicent.h"

/*
** DcmDictEntry member functions
*/

static
char* strdup_new(const char* str)
{
    char* s = NULL;
    if (str != NULL) {
        s = new char[strlen(str)+1];
        strcpy(s, str);
    }
    return s;
}

DcmDictEntry::DcmDictEntry(Uint16 g, Uint16 e, DcmVR vr, 
        const char* nam, int vmMin, int vmMax,
        const char* vers, OFBool doCopyStrings, 
        const char* pcreator)
  : DcmTagKey(g,e),
    upperKey(),
    valueRepresentation(EVR_UNKNOWN),
    tagName(nam),
    valueMultiplicityMin(vmMin),
    valueMultiplicityMax(vmMax),
    standardVersion(vers),
    stringsAreCopies(doCopyStrings),
    groupRangeRestriction(DcmDictRange_Unspecified),
    elementRangeRestriction(DcmDictRange_Unspecified),
    privateCreator(pcreator)
{
    upperKey.set(g,e);  /* default: make upper key same as normal key */
    valueRepresentation.setVR(vr);
    if (doCopyStrings) {
        tagName = strdup_new(nam);
        standardVersion = strdup_new(vers);
        privateCreator = strdup_new(pcreator);
    }
}

DcmDictEntry::DcmDictEntry(Uint16 g, Uint16 e, Uint16 ug, Uint16 ue, DcmVR vr, 
        const char* nam, int vmMin, int vmMax,
        const char* vers, OFBool doCopyStrings, 
        const char* pcreator)
  : DcmTagKey(g,e),
    upperKey(),
    valueRepresentation(EVR_UNKNOWN),
    tagName(nam),
    valueMultiplicityMin(vmMin),
    valueMultiplicityMax(vmMax),
    standardVersion(vers),
    stringsAreCopies(doCopyStrings),
    groupRangeRestriction(DcmDictRange_Unspecified),
    elementRangeRestriction(DcmDictRange_Unspecified),
    privateCreator(pcreator)
{
    upperKey.set(ug, ue);
    valueRepresentation.setVR(vr);
    if (doCopyStrings) {
        tagName = strdup_new(nam);
        standardVersion = strdup_new(vers);
        privateCreator = strdup_new(pcreator);
    }
}

DcmDictEntry::DcmDictEntry(const DcmDictEntry& e)
  : DcmTagKey(e),
    upperKey(e.upperKey),
    valueRepresentation(e.valueRepresentation),
    tagName(e.tagName),
    valueMultiplicityMin(e.valueMultiplicityMin),
    valueMultiplicityMax(e.valueMultiplicityMax),
    standardVersion(e.standardVersion),
    stringsAreCopies(e.stringsAreCopies),
    groupRangeRestriction(e.groupRangeRestriction),
    elementRangeRestriction(e.elementRangeRestriction),
    privateCreator(e.privateCreator)
{
    if (stringsAreCopies)
    {
        tagName = strdup_new(e.tagName);
        standardVersion = strdup_new(e.standardVersion);
        privateCreator = strdup_new(e.privateCreator);
    }
}


DcmDictEntry::~DcmDictEntry()
{
    if (stringsAreCopies) {
        /* we have allocated them so it is ok to deallocate them */
        delete[] OFconst_cast(char *, tagName);
        delete[] OFconst_cast(char *, standardVersion);
        delete[] OFconst_cast(char *, privateCreator);
    }
}

/*
** DcmDictEntry friend functions
*/

ostream& operator<<(ostream& s, const DcmDictEntry& e) {

    const DcmDictEntry* ep = &e;
    if (ep == NULL) {
        s << "(nil)";
        return s;
    }

    char tagBuf[16];

    sprintf(tagBuf, "(%04x", e.getGroup());
    s << tagBuf;
    if (e.isRepeatingGroup()) {
        sprintf(tagBuf, "-%04x", e.getUpperGroup());
        s << tagBuf;
    }
    sprintf(tagBuf, ",%04x", e.getElement());
    s << tagBuf;
    if (e.isRepeatingElement()) {
        sprintf(tagBuf, "-%04x", e.getUpperElement());
        s << tagBuf;
    }
    s << ")";


    s << " " << e.getVR().getVRName() << " \"" << e.getTagName() << "\" ";
    if (e.isFixedSingleVM()) {
        s << "vm=" << e.getVMMax() << " ";
    } else if (e.isVariableRangeVM()) {
        s << "vm=" << e.getVMMin() << "-n ";
    } else if (e.isFixedRangeVM()){
        s << "vm=" << e.getVMMin() << "-" << e.getVMMax() << " ";
    } else {
        s << "vm=?(" <<  e.getVMMin() << "-" << e.getVMMax() << ")? ";
    }
    if (e.getStandardVersion() != NULL) {
        s << " Version=\"" << e.getStandardVersion() << "\" ";
    }
    if (e.getPrivateCreator() != NULL) {
        s << " priv=\"" << e.getPrivateCreator() << "\" ";
    }

    return s;
}

/*
** CVS/RCS Log:
** $Log: dcdicent.cc,v $
** Revision 1.2  2007/04/24 09:53:25  braindead
** - updated DCMTK to version 3.5.4
** - merged Gianluca's WIN32 changes
**
** Revision 1.1.1.1  2006/07/19 09:16:40  pipelka
** - imported dcmtk354 sources
**
**
** Revision 1.14  2005/12/08 15:41:03  meichel
** Changed include path schema for all DCMTK header files
**
** Revision 1.13  2004/02/04 16:26:07  joergr
** Adapted type casts to new-style typecast operators defined in ofcast.h.
**
** Revision 1.12  2002/11/27 12:06:44  meichel
** Adapted module dcmdata to use of new header file ofstdinc.h
**
** Revision 1.11  2002/07/23 14:21:29  meichel
** Added support for private tag data dictionaries to dcmdata
**
** Revision 1.10  2001/06/01 15:49:01  meichel
** Updated copyright header
**
** Revision 1.9  2000/03/08 16:26:32  meichel
** Updated copyright header.
**
** Revision 1.8  2000/02/02 14:32:49  joergr
** Replaced 'delete' statements by 'delete[]' for objects created with 'new[]'.
**
** Revision 1.7  1999/03/31 09:25:21  meichel
** Updated copyright header in module dcmdata
**
** Revision 1.6  1998/07/15 15:51:50  joergr
** Removed several compiler warnings reported by gcc 2.8.1 with
** additional options, e.g. missing copy constructors and assignment
** operators, initialization of member variables in the body of a
** constructor instead of the member initialization list, hiding of
** methods by use of identical names, uninitialized member variables,
** missing const declaration of char pointers. Replaced tabs by spaces.
**
** Revision 1.5  1997/08/26 13:48:27  hewett
** Modified constructors to take const reference parameters.
**
** Revision 1.4  1997/07/31 14:40:36  meichel
** Created copy constructor for class DcmDictEntry, required by dcmcheck.
**
** Revision 1.3  1997/07/21 08:25:24  andreas
** - Replace all boolean types (BOOLEAN, CTNBOOLEAN, DICOM_BOOL, BOOL)
**   with one unique boolean type OFBool.
**
** Revision 1.2  1996/03/20 16:44:04  hewett
** Updated for revised data dictionary.  Repeating tags are now handled better.
** A linear list of repeating tags has been introduced with a subset ordering
** mechanism to ensure that dictionary searches locate the most precise
** dictionary entry.
**
*/
