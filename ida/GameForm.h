typedef unsigned char		UInt8;		//!< An unsigned 8-bit integer value
typedef unsigned short		UInt16;		//!< An unsigned 16-bit integer value
typedef unsigned long		UInt32;		//!< An unsigned 32-bit integer value
typedef unsigned long long	UInt64;		//!< An unsigned 64-bit integer value
typedef signed char			SInt8;		//!< A signed 8-bit integer value
typedef signed short		SInt16;		//!< A signed 16-bit integer value
typedef signed long			SInt32;		//!< A signed 32-bit integer value
typedef signed long long	SInt64;		//!< A signed 64-bit integer value
typedef float				Float32;	//!< A 32-bit floating point value
typedef double				Float64;	//!< A 64-bit floating point value

struct BSString
{
    char	* m_data;	// 00
    UInt16	m_dataLen;	// 08
    UInt16	m_bufLen;	// 0A
    UInt32	pad0C;		// 0C
};

struct NiPoint3 {
    float x, y, z;
};

struct ModInfo		// referred to by game as TESFile
{
    UInt64								unk00[10];			// 000
    struct BSFile*						file;				// 050
    UInt64								unk58;				// 058
    UInt64								unk60;				// 060
    void								* unk68;			// 068
    char								name[MAX_PATH];		// 070
    char								directory[MAX_PATH];	// 174
    UInt64								unk278[0xD8 / 8];		// 278
    struct Dependency
    {
        void		* unk00;	// 00
        const char	* name;		// 08
        struct Dependency	* next;		// 10
    };
    Dependency							* depends;			// 350
    UInt32								numRefMods;			// 358
    UInt32								unk35C;				// 35C
    struct ModInfo								** refModInfo;		// 360
    UInt64								unk368;				// 360
    UInt8								modIndex;			// 370
    UInt8								pad[7];				// 371
    struct BSString							author;				// 378
    struct BSString							description;		// 388
    // ...
};

struct BaseFormComponent__vtbl {
    void(*destructor)(void);
    void(*Unk_01)(void);
    void(*Unk_02)(void);
    void(*Unk_03)(void);
    void(*Unk_04)(void);
    void(*Unk_05)(void);
    void(*Unk_06)(void);
};

struct TESForm__vtbl {
    struct BaseFormComponent__vtbl _pvtbl;
    void	(*Unk_07)(void);
    void	(*Unk_08)(void);
    void	(*Unk_09)(void);
    void	(*Unk_0A)(void);
    void	(*Unk_0B)(void);
    void	(*Unk_0C)(void);
    bool	(*MarkChanged)(UInt64 changed);
    void	(*Unk_0E)(void);
    void	(*Unk_0F)(void);
    void	(*Unk_10)(void);
    void	(*Unk_11)(void); // Serialize
    void	(*Unk_12)(void);
    void	(*Unk_13)(void);
    void	(*Unk_14)(void);
    void	(*Unk_15)(void);
    void	(*Unk_16)(void);
    struct ModInfo*  (*GetLastModifiedMod)(void); // 17 - Returns the ModInfo* of the mod that last modified the form.
    struct ModInfo*  (*GetLastModifiedMod_2)(void); // 18 - Returns the ModInfo* of the mod that last modified the form. Calls a helper function to do so.
    UInt8     (*GetFormType)(void);  // 19
    void      (*Unk_1A)(void); // 1A - GetDebugName(TESForm * form, char * destBuffer, unsigned int bufferSize);
    bool      (*GetPlayerKnows)(void); // 1B - Gets flag bit 6.
    void	(*Unk_1C)(void);
    void	(*Unk_1D)(void);
    void	(*Unk_1E)(void);
    void	(*Unk_1F)(void);
    void	(*Unk_20)(void);
    void	(*Unk_21)(void);
    void	(*Unk_22)(void);
    void	(*Unk_23)(void);
    void	(*Unk_24)(void);
    void	(*Unk_25)(void);
    void	(*Unk_26)(void);
    void	(*Unk_27)(void);
    void	(*Unk_28)(void);
    void	(*Unk_29)(void);
    void	(*Unk_2A)(void);
    void	(*Unk_2B)(void);
    void	(*Unk_2C)(void);
    void	(*Unk_2D)(void);
    void	(*Unk_2E)(void);
    void	(*Unk_2F)(void);
    void	(*Unk_30)(void);
    void	(*Unk_31)(void);
    void	(*Unk_32)(void);
    void	(*Unk_33)(void);
    void	(*Unk_34)(void);
    const char *(*GetFullName)(void);
    void	(*Unk_36)(void);
    void	(*Unk_37)(void);
    void	(*Unk_38)(void);
    void	(*Unk_39)(void);
    const char *(*GetEditorID)(void);
    void	(*Unk_3B)(void);
    void	(*Unk_3C)(void);
    void	(*Unk_3D)(void);
    void	(*Unk_3E)(void);
    void	(*Unk_3F)(void);
    void	(*Unk_40)(void);
    void	(*Unk_41)(void);
    void	(*Unk_42)(void);
    void	(*Unk_43)(void);
    void	(*Unk_44)(void);
    void	(*Unk_45)(void);
    void	(*Unk_46)(void);
    void	(*Unk_47)(void);

};

struct TESObjectREFR__vtbl
{
    void(*Unk_48)(void);
    void(*Unk_49)(void);
    void(*Unk_4A)(void);
    void(*Unk_4B)(void);
    void(*Unk_4C)(void);
    void(*Unk_4D)(void);
    void(*Unk_4E)(void);
    void(*Unk_4F)(void);
    void(*Unk_50)(void);
    void(*Unk_51)(void);
    void(*Unk_52)(void);
    void(*Unk_53)(void);
    void(*Unk_54)(void);
    void(*Unk_55)(void);
    void(*Unk_56)(void);
    void(*Unk_57)(void);
    void(*Unk_58)(void);
    void(*Unk_59)(void);
    void(*Unk_5A)(void);
    void(*Unk_5B)(void);
    struct BGSScene* (*GetCurrentScene)(void);  // 5C  Returns the Scene this reference is currently participating in, or NULL if it isn't in a scene.
    void(*Unk_5D)(void);
    void(*Unk_5E)(void);
    void(*Unk_5F)(void);
    void(*Unk_60)(void);
    void(*Unk_61)(void);
    void(*Unk_62)(void);
    void(*Unk_63)(void);
    void(*Unk_64)(void);
    void(*Unk_65)(void);
    void(*Unk_66)(void);
    void(*Unk_67)(void);
    void(*Unk_68)(void);
    void(*Unk_69)(void);
    void(*Unk_6A)(void);
    void(*Unk_6B)(void);
    void(*Unk_6C)(void);
    void(*Unk_6D)(void);
    void(*Unk_6E)(void);
    void(*Unk_6F)(void);
    void(*Unk_70)(void);
    void(*Unk_71)(void);
    void(*Unk_72)(void);
    void(*Unk_73)(void);
    void(*Unk_74)(void);
    void(*Unk_75)(void);
    void(*Unk_76)(void);
    void(*Unk_77)(void);
    void(*Unk_78)(void);
    void(*Unk_79)(void);
    void(*Unk_7A)(void);
    void(*GetMarkerPosition)(struct NiPoint3 * pos);
    void(*Unk_7C)(void);
    void(*Unk_7D)(void);
    void(*Unk_7E)(void);
    void(*Unk_7F)(void);
    void(*Unk_80)(void);
    void(*Unk_81)(void);
    void(*Unk_82)(void);
    void(*Unk_83)(void);
    void(*Unk_84)(void);
    void(*Unk_85)(void);
    void(*Unk_86)(void);
    void(*Unk_87)(void);
    void(*Unk_88)(void);
    void(*Unk_89)(void);
    void(*Unk_8A)(void);
    struct NiNode *(*GetActorRootNode)(bool firstPerson);	// 8B - Returns either first person or third person
    struct NiNode *(*GetObjectRootNode)(void); // 8C - Returns the 3rd person skeleton
    void(*Unk_8D)(void);
    void(*Unk_8E)(void);
    void(*Unk_8F)(void);
    void(*Unk_90)(void);
    struct TESRace *(*GetActorRace)(void); // 91
    void(*Unk_92)(void);
    void(*Unk_93)(void);
    void(*Unk_94)(void);
    void(*Unk_95)(void);
    void(*Unk_96)(void);
    void(*Unk_97)(void);
    void(*Unk_98)(void);
    void(*Unk_99)(void);
    void(*Unk_9A)(void);
    void(*Unk_9B)(void);
    void(*Unk_9C)(void);
    void(*Unk_9D)(void);
    void(*Unk_9E)(void);
    void(*Unk_9F)(void);
    struct ActorEquipData**(*GetEquipData)(bool bFirstPerson);
    void(*Unk_A1)(void);
    void(*Unk_A2)(void);
    void(*Unk_A3)(void);
    void(*Unk_A4)(void);
    void(*Unk_A5)(void);
    void(*Unk_A6)(void);
    void(*Unk_A7)(void);
    void(*Unk_A8)(void);
    void(*Unk_A9)(void);
    void(*Unk_AA)(void);
    void(*Unk_AB)(void);
    void(*Unk_AC)(void);
    void(*Unk_AD)(void);
    void(*Unk_AE)(void);
    void(*Unk_AF)(void);
    void(*Unk_B0)(void);
    void(*Unk_B1)(void);
    void(*Unk_B2)(void);
    void(*Unk_B3)(void);
    void(*Unk_B4)(void);
    void(*Unk_B5)(void);
    void(*Unk_B6)(void);
    void(*Unk_B7)(void);
    void(*Unk_B8)(void);
    void(*Unk_B9)(void);
    void(*Unk_BA)(void);
    void(*Unk_BB)(void);
    void(*Unk_BC)(void);
    void(*Unk_BD)(void);
    void(*Unk_BE)(void);
    void(*Unk_BF)(void);
    void(*Unk_C0)(void);
    void(*Unk_C1)(void);
    void(*Unk_C2)(void);
    void(*Unk_C3)(void);
};

struct TESForm__data {
    struct Data
    {
        struct ModInfo ** entries;     // array of ModInfo* - mods that change this form.
        UInt64  size;
    };

    struct Data	*unk08;	// 08
    UInt32	flags;		// 10
    UInt32	formID;		// 14
    UInt16	unk18;		// 18
    UInt8	formType;	// 1A
    UInt8	unk1B;		// 1B
    UInt32	pad1C;		// 1C

};

struct TESForm {
    struct TESForm__vtbl *_vtbl;
    struct TESForm__data _data;
};

struct BSHandleRefObject {
    void	**_vtbl;		// 00
    SInt32	m_uiRefCount;	// 08
};

struct TESObjectCELL {
    struct TESForm form;
};

struct TESObjectREFR__data {
    struct TESForm__data form;
    // parents
    struct BSHandleRefObject							handleRefObject;		// 20
    void 	*activeGraphIfInactive;	// 30
    void    *animGraphEventSink;	// 38
    void    *inventoryListSink;		// 40
    void	*animGraphHolder;		// 48
    void	*keywordFormBase;		// 50
    void	*actorValueOwner;		// 58
    void										* unk60;				// 60
    void										* unk68;				// 68
    UInt32										unk70;					// 70
    UInt32										unk74;					// 74
    UInt32										unk78;					// 78
    UInt32										unk7C;					// 7C
    UInt64										unk80;					// 80
    UInt64										unk88;					// 88
    UInt64										unk90;					// 90
    UInt64										unk98;					// 98
    UInt64										unkA0;					// A0
    UInt64										unkA8;					// A8
    UInt64										unkB0;					// B0
    struct TESObjectCELL								* parentCell;			// B8
    struct NiPoint3									rot;					// C0, C4, C8 - Probably quat?
    float										unkCC;
    struct NiPoint3									pos;					// D0, D4, D8
    float										unkDC;
    struct TESForm										* baseForm;				// E0
    void										* unkE8;				// E8

    struct LoadedData
    {
        UInt64	unk00;
        struct NiNode	* rootNode;
        UInt64	unk10;
        UInt64	unk18;

        enum
        {
            kFlag_PhysicsInitialized = 1
        };

        UInt64	flags;
        // ...
    };

    struct LoadedData							* unkF0;				// F0 - Root node at 0x08
    void							* inventoryList;		// F8
    void								* extraDataList;		// 100 - ExtraData?
    float										scale;					// 104
    UInt32										unk108;					// 108
};

struct TESObjectREFR {
    struct TESObjectREFR__vtbl *_vtbl;
    struct TESObjectREFR__data data;
};