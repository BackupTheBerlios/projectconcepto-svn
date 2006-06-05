#include "ProjectConceptorDefs.h"
#include "RemoveAttribute.h"


RemoveAttribute::RemoveAttribute():PCommand()
{
}

void RemoveAttribute::Undo(PDocument *doc,BMessage *undo)
{
	BMessage	*undoMessage	= new BMessage();
	BMessage	*selectNodes	= new BMessage();
	BMessage	*valueContainer	= new BMessage();
	BMessage	*node			= NULL;
	bool		selected		= false;
	int32		i				= 0;
	status_t	err				= B_OK;

	PCommand::Undo(doc,undoMessage);
	undo->FindMessage("AddAttribute::Undo" ,undoMessage);
	while (undo->FindPointer("node",i,(void **)&node) == B_OK)
	{
		err = B_OK;
		if (undo->FindMessage("valueContainer",i,valueContainer) == B_OK)
			AddAttribute(doc,node,valueContainer,newValue);
		i++;

	}
	if ( (undo->FindBool("selected",&selected) == B_OK) && (selected==true) )
	{
		undoMessage->FindMessage("selectedNodes",selectNodes);
		err 		= B_OK;
		i			= 0;
		if (undo->FindMessage("valueContainer",valueContainer) == B_OK)
		{
			while (selectNodes->FindPointer("node",i,(void **)&node) == B_OK)
			{
				AddAttribute(doc,node,valueContainer,newValue);
				i++;				
			}
		}	
	}
	doc->SetModified();
}

BMessage* RemoveAttribute::Do(PDocument *doc, BMessage *settings)
{
	BList		*selection		= doc->GetSelected();
	BMessage	*undoMessage	= new BMessage();
	BMessage	*node			= NULL;
	BMessage	*selectedNodes	= new BMessage();
	BMessage	*valueContainer	= new BMessage();
	int32		i				= 0;
	bool		selected		= false;
	status_t	err				= B_OK;

	while (settings->FindPointer("node",i,(void **)&node) == B_OK)
	{
		if (settings->FindMessage("valueContainer",i,valueContainer) == B_OK)
			RemoveAttribute(doc,node,valueContainer,undoMessage);
		i++;
	}
	if ( (settings->FindBool("selected",&selected) == B_OK) && (selected==true) )
	{
		if (settings->FindMessage("valueContainer",i,valueContainer) == B_OK)
		{
			for (int32 i=0;i<selection->CountItems();i++)
			{
				node =(BMessage *) selection->ItemAt(i);
				selectedNodes->AddPointer("node",node);
				RemoveAttribute(doc,node,valueContainer,selectedNodes);
			}
		}
	}
	undoMessage->AddMessage("selectedNodes",selectedNodes);
	doc->SetModified();
	settings->RemoveName("RemoveAttribute::Undo");
	settings->AddMessage("RemoveAttribute::Undo",undoMessage);
	settings = PCommand::Do(doc,settings);
	return settings;



void RemoveAttribute::AttachedToManager(void)
{
}

void RemoveAttribute::DetachedFromManager(void)
{
}

void RemoveAttribute::RemoveAttribute(PDocument *doc, BMessage *node, BMessage *valueContainer,BMessage *undoMessage)
{
	status_t	err				= B_OK;
	int32 		i				= 0;
	BList		*subGroupList	= new BList();
	BMessage	*subGroup		= NULL;
	BMessage	*tmpSubGroup	= new BMessage();
	BList		*changed		= doc->GetChangedNodes();
	//do 
	char		*name			= NULL;
	char		*subGroupName	= NULL;
	type_code	type			= B_ANY_TYPE;
	void*		newValue		= NULL;
	int32		index			= NULL;
	err = valueContainer->FindString("name",(const char**)&name);
	err = err | valueContainer->FindInt32("type",(int32 *)&type);
	err = err | valueContainer->FindInt32("index",(int32 *)&type);
	subGroup = node;
	subGroupList->AddItem(subGroup);
	while (valueContainer->FindString("subgroup",i,(const char**)&subGroupName) == B_OK)
	{	
		subGroup->FindMessage(subGroupName,tmpSubGroup);
		subGroupList->AddItem(tmpSubGroup);
		subGroup	= tmpSubGroup;
		tmpSubGroup	= new BMessage();
		i++;
	}
	delete tmpSubGroup;
	subGroup->AddData(name,type,newValue,size);
	for (i=subGroupList->CountItems()-1;i>0;i--)
	{
		tmpSubGroup = (BMessage *)subGroupList->ItemAt(i-1);
		valueContainer->FindString("subgroup",i-1,(const char**)&subGroupName);
		if (tmpSubGroup)
			tmpSubGroup->ReplaceMessage(subGroupName,(BMessage *)subGroupList->ItemAt(i));
		delete subGroupList->RemoveItem(i);
	}
	changed->AddItem(node);
	
}

void RemoveAttribute::DoAddAttribute(PDocument *doc, BMessage *node, BMessage *valueContainer,BMessage *newValue)
{
	int32 		i				= 0;
	status_t	err				= B_OK;
	BList		*subGroupList	= new BList();
	BMessage	*subGroup		= NULL;
	BMessage	*tmpSubGroup	= new BMessage();
	BList		*changed		= doc->GetChangedNodes();
	//do 
	char		*name			= NULL;
	char		*subGroupName	= NULL;
	type_code	type			= B_ANY_TYPE;
	void*		newValue		= NULL;
	ssize_t		size			= 0;
	
	//undo
	char		*compareName	= NULL;
	int32		lastIndex		= -1;
	int32		count			= 0;
	int32		index			= 0;
	type_code	typeFound		= B_ANY_TYPE;

	err = valueContainer->FindString("name",(const char**)&name);
	err = err | valueContainer->FindInt32("type",(int32 *)&type);
	err = valueContainer->FindData("newAttribute", type,(const void **)&newValue, &size);
	subGroup = node;
	subGroupList->AddItem(subGroup);
	while (valueContainer->FindString("subgroup",i,(const char**)&subGroupName) == B_OK)
	{	
		subGroup->FindMessage(subGroupName,tmpSubGroup);
		subGroupList->AddItem(tmpSubGroup);
		subGroup	= tmpSubGroup;
		tmpSubGroup	= new BMessage();
		i++;
	}
	delete tmpSubGroup;
	subGroup->AddData(name,type,newValue,size);
	for (i=subGroupList->CountItems()-1;i>0;i--)
	{
		tmpSubGroup = (BMessage *)subGroupList->ItemAt(i-1);
		valueContainer->FindString("subgroup",i-1,(const char**)&subGroupName);
		if (tmpSubGroup)
			tmpSubGroup->ReplaceMessage(subGroupName,(BMessage *)subGroupList->ItemAt(i));
		delete subGroupList->RemoveItem(i);
	}
	changed->AddItem(node);
}