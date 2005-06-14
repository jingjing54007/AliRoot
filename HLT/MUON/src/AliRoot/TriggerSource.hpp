////////////////////////////////////////////////////////////////////////////////
//
// Author: Artur Szostak
// Email:  artur@alice.phy.uct.ac.za | artursz@iafrica.com
//
////////////////////////////////////////////////////////////////////////////////

#ifndef dHLT_ALIROOT_TRIGGER_SOURCE_HPP
#define dHLT_ALIROOT_TRIGGER_SOURCE_HPP

#include "TROOT.h"
#include "TObject.h"
#include "TString.h"
#include "TClonesArray.h"
#include "AliRoot/TriggerRecord.hpp"

class AliMUON;
class AliMUONLocalTrigger;
class AliMUONDataInterface;


namespace AliMUONHLT
{


class TriggerSource : public TObject
{
public:

	enum AreaType
	{
		FromWholePlane,
		FromLeftHalfPlane,
		FromRightHalfPlane
	};
	
	enum SourceType
	{
		FromHits,
		FromLocalTriggers
	};


	TriggerSource();
	
	/* Creates a new trigger source object by filling data from the data interface.
	 */
	TriggerSource(AliMUONDataInterface* data);
	
	virtual ~TriggerSource();
	
	/* Get and set methods to specify how the FillFrom methods should fill the
	   internal data structures.
	 */
	void AreaToUse(const AreaType value) { fAreaToUse = value; };
	AreaType AreaToUse() const { return fAreaToUse; };
	void DataToUse(const SourceType value) { fDataToUse = value; };
	SourceType DataToUse() const { return fDataToUse; };
	void MaxBlockSize(const UInt_t value) { fMaxBlockSize = value; };
	UInt_t MaxBlockSize() const { return fMaxBlockSize; };
	void UseLookupTable(const Bool_t value) { fUseLookupTable = value; };
	Bool_t UseLookupTable() const { return fUseLookupTable; };
	
	/* Fills the internal data structures from the specified data interface
	   for all the events found in AliMUONDataInterface.
	 */
	void FillFrom(AliMUONDataInterface* data);
	
	/* Fills the internal data structures from the specified data interface
	   for the given event.
	 */
	void FillFrom(AliMUONDataInterface* data, const Int_t event);
	
	/* Fills the internal data structures from the specified data interface
	   for the given event and trigger number.
	   If 'newblock' is set to true then the new trigger record is added to 
	   a new block. Otherwise the point is added to the current block.
	   Note: This method ignores the fAreaToUse and fMaxBlockSize flags.
	   This is very usefull for custom trigger source filling.
	   For the case of adding data from AliMUONHit objects the 'trigger'
	   parameter becomes the track number in TreeH and not the index of the
	   AliMUONLocalTrigger object.
	 */
	void FillFrom(
			AliMUONDataInterface* data, 
			const Int_t event, const Int_t trigger, const Bool_t newblock = kFALSE
		);

	/* Clears all the internal arrays.
	 */
	void Clear();
	
	// Get methods.
	TString FileName()   const { return fFilename; };
	TString FolderName() const { return fFoldername; };
	
	/* Returns the number of events stored.
	 */
	Int_t NumberOfEvents() const { return fEventList.GetEntriesFast(); };
	
	/* Fetches the specified event number stored in this TriggerSource.
	   Sets the current block and trigger to the first block and trigger record in
	   the event. If there are no blocks or trigger records then these pointers are
	   set to NULL. kTRUE is returned if the event was found, kFALSE otherwise.
	 */
	Bool_t GetEvent(const Int_t eventnumber) const;
	
	/* Fetches the first event stored in this TriggerSource.
	   Sets the current block and trigger record to the first block and trigger
	   in the event. If there are no blocks or trigger records then these pointers
	   are set to NULL. kTRUE is returned if the event was found, kFALSE otherwise.
	 */
	Bool_t GetFirstEvent() const;
	
	/* Returns kTRUE if there are more events to iterate over.
	 */
	Bool_t MoreEvents() const;
	
	/* Fetches the next event stored following the currently selected one.
	   kTRUE is returned if the event was found, kFALSE otherwise.
	   The internal pointers are reset if we reached the last event.
	 */
	Bool_t GetNextEvent() const;
	
	/* Returns the corresponding AliRoot event number for the current event.
	   -1 is returned if no event is selected.
	 */
	Int_t CurrentEvent() const;
	
	/* Returns the number of trigger record blocks in the current event.
	   -1 is returned if no event is selected.
	 */
	Int_t NumberOfBlocks() const;
	
	/* Fetches the index'th block in the current event.
	   Sets the current trigger record to the first trigger in the block.
	   If there are no trigger records then this pointer is set to NULL.
	   kTRUE is returned if the block was found, kFALSE otherwise.
	 */
	Bool_t GetBlock(const Int_t index) const;
	
	/* Fetches the first block in the current event.
	   Sets the current trigger record to the first trigger in the block.
	   If there are no trigger records then this pointer is set to NULL.
	   kTRUE is returned if the block was found, kFALSE otherwise.
	 */
	Bool_t GetFirstBlock() const;

	/* Returns kTRUE if there are more blocks to be traversed.
	 */
	Bool_t MoreBlocks() const;
	
	/* Fetches the next block in the current event.
	   kTRUE is returned if the block was found, kFALSE otherwise.
	 */
	Bool_t GetNextBlock() const;
	
	/* Returns the currently selected block number.
	   -1 is returned if no blocks are selected.
	 */
	Int_t CurrentBlock() const { return fBlockIndex; };
	
	/* Returns the number of trigger records in the current block.
	   -1 is returned if no block is selected.
	 */
	Int_t NumberOfTriggers() const;
	
	/* Fetches the trigger record with the specified trigger number from
	   the current block.
	   NULL is returned if the record was not found.
	 */
	const TriggerRecord* GetTrigger(const Int_t triggernumber) const;
	
	/* Fetches the first trigger record in the current block.
	   NULL is returned if the record was not found.
	 */
	const TriggerRecord* GetFirstTrigger() const;
	
	/* Returns kTRUE if there are more triggers to iterate over.
	 */
	Bool_t MoreTriggers() const;
	
	/* Fetches the next trigger record in the current block.
	   NULL is returned if the record was not found.
	 */
	const TriggerRecord* GetNextTrigger() const;
	
	/* Returns the current trigger record.
	   NULL is returned if the record was not found.
	 */
	const TriggerRecord* GetTrigger() const { return fCurrentTrigger; };
	
	/* Returns the trigger record number for the currently selected trigger record.
	   This number corresponds to the index'th AliMUONLocalTrigger object for the
	   current event.
	   -1 is returned if no trigger record is selected.
	 */
	Int_t CurrentTrigger() const;


private:

	/* Adds a new EventData block to the fEventList and updates the fCurrentEvent,
	   fCurrentBlock and fCurrentTrigger pointers.
	 */ 
	void AddEvent(const Int_t eventnumber);
	
	/* Adds a new block to the current event and updates fCurrentBlock and fCurrentTrigger.
	 */
	void AddBlock();
	
	/* Adds a new trigger record to the current event and block.
	   The fCurrentTrigger is updated appropriately.
	 */
	void AddTrigger(const TriggerRecord& data);
	
	/* Checks if the file and folder names correspond to this TriggerSource's 
	   file and folder names. kTRUE is returned if they do.
	   If the file and folder names are empty then they are assigned the names
	   as found in the data interface and kTRUE is returned.
	 */
	Bool_t FileAndFolderOk(AliMUONDataInterface* data);
	
	/* Adds the whole event from the data interface to the internal data structures.
	   It is assumed that FileAndFolderOk(data) returns true just before calling
	   this method.
	 */
	void AddEventFrom(AliMUONDataInterface* data, AliMUON* module, const Int_t event);
	
	/* Adds the specified trigger record from the given data interface.
	   The data interface should be initialised correctly, that is the event
	   should already be selected before calling this method.
	 */
	void AddTriggerFrom(AliMUONDataInterface* data, AliMUON* module, const Int_t trigger);
	
	/* Checks to see if the specified trigger record is in the chamber region
	   we want to fill from.
	   kTRUE is returned if (x, y) is in the region, and kFALSE otherwise.
	 */
	Bool_t InFillRegion(const TriggerRecord& data);
	
	/* Fills the trigger data from the AliMUONLocalTrigger object.
	   if the fUseLookupTable is set to true then we use the L0 lookup table to
	   fill the Pt value otherwise we use the PtCal method in AliMUONTriggerCircuit.
	   Note the fTriggerNumber parameter is not filled in to 'record'.
	 */
	void FillTriggerFromLocalTrigger(
			AliMUONLocalTrigger* trigger, AliMUON* module, TriggerRecord& record
		);
	
	/* Fills the TriggerRecord structure from AliMUONHit objects.
	   The hits on the last 4 chambers are used (i.e. chambers 11 to 14).
	   kTRUE is returned if the structure was filled successfully.
	 */
	Bool_t FillTriggerFromHits(AliMUONDataInterface* data, const Int_t track, TriggerRecord& record);
	
	/* Fetches the AliMUON module from the AliRun global object. AliRun will be loaded
	   by the runloader if it has not yet been loaded. In such a case the AliRun object
	   will also we unloaded when we are done with it.
	   kTRUE is returned if no error occured and kFALSE otherwise.
	   Note that if fDataToUse is set to FromHits then gAlice is not loaded and 'module'
	   will be left untouched. The method will still return kTRUE however since this is
	   not an error. We do not need the AliMUON module when filling from hits.
	 */
	Bool_t FetchAliMUON(AliMUON*& module);
	
	/* After one is finished with the AliMUON object returned by GetAliMUON, one
	   should call this method.
	   If the gAlice object was loaded by GetAliMUON then it will be unloaded at
	   this point, otherwise nothing is done.
	 */
	void FinishedWithAliMUON();

	/* Sets all the current pointers to NULL and indices to -1.
	 */
	void ResetAllPointers() const;
	
	/* Sets the block and trigger pointers to NULL and indices to -1.
	 */
	void ResetBlockPointers() const;
	
	/* Sets just the current trigger record pointer to NULL and index to -1.
	 */
	void ResetTriggerPointers() const;


public:  // Unfortunately ROOT requires the following to be public.

	class EventData : public TObject
	{
	public:
		EventData();
		EventData(const Int_t eventnumber);
		virtual ~EventData();

		Int_t fEventNumber;  // Event number in AliMUONDataInterface from which the triggers were taken.
		TClonesArray fBlocks; // The list of blocks of trigger records.
		
		ClassDef(EventData, 1);  // Data per event.
	};
	
private:

	AreaType fAreaToUse;    //! The part of the chamber to fill from.
	SourceType fDataToUse;  //! The type of raw AliRoot data to fill from.
	UInt_t fMaxBlockSize;   //! The maximum block size to create in the fill methods.
	Bool_t fUseLookupTable; //! Set to true if the L0 lookup table should be used for finding Pt.

	TString fFilename;    // The file from which the trigger data was taken.
	TString fFoldername;  // The folder name from which trigger data was taken.
	
	mutable Int_t fEventIndex;               //! The index number of the currently selected event.
	mutable EventData* fCurrentEvent;        //! Pointer to the currently selected event.
	mutable Int_t fBlockIndex;               //! The index number of the currently selected block.
	mutable TClonesArray* fCurrentBlock;     //! Pointer to the currently selected block.
	mutable Int_t fTriggerIndex;             //! The index number of the currently selected trigger record.
	mutable TriggerRecord* fCurrentTrigger;  //! Pointer to the currently selected trigger record.

	TClonesArray fEventList;   // List of trigger records per event.
	
	Bool_t fHadToLoadgAlice;  //! Flag indicating if this object had to load the AliRun object.

	ClassDef(TriggerSource, 1);  // The source of trigger records for dHLT.
};


}; // AliMUONHLT

#endif // dHLT_ALIROOT_TRIGGER_SOURCE_HPP
