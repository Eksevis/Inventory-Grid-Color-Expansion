/////////////////////////////////////
// NEW ADDITIONS START AT LINE 224 //
/////////////////////////////////////

#include "dsdll.h"
// #include "gpstring_std.h"

{
	
#	if !GP_RETAIL
	// Register console command from this UI layer
	m_pConsoleCommands		= new UIGame_console_commands;

	m_pPolyText = 0;
	m_bPolyAlert = false;

	// Initialize the LOD to Polygon count map
	FastFuelHandle hPolyCount( "config:debug_polymap" );
	if ( hPolyCount.IsValid() )
	{
		FastFuelFindHandle hFind( hPolyCount );
		if ( hFind.IsValid() )
		{
			if ( hFind.FindFirstKeyAndValue() )
			{
				gpstring sKey;
				gpstring sValue;
				while ( hFind.GetNextKeyAndValue( sKey, sValue ) )
				{
					float lod = 0.0f;
					int polys = 0;
					stringtool::Get( sKey, lod );
					stringtool::Get( sValue, polys );
					
					m_LodToPolyMap.insert( std::make_pair( lod, polys ) );
				}
			}
		}
	}

#	endif // !GP_RETAIL

	// Initialize our camera
	m_pUICamera				= new UICamera;

	// Initialize our command handler
	m_pUICommands			= new UICommands;

	// Initialize our helpers
	m_pUIPartyManager		= new UIPartyManager;
	m_pUIInventoryManager	= new UIInventoryManager;
	m_pUIMenuManager		= new UIMenuManager;
	m_pUIStoreManager		= new UIStoreManager;
	m_pUIGameConsole		= new UIGameConsole;
	m_pUIPlayerRanks		= new UIPlayerRanks;
	m_pUIEmoteList			= new UIEmoteList;
	m_pUITeamManager		= new UITeamManager;	

	// Input Bindings
	m_pGameInputBinder		= new InputBinder( "ingamedefaults" );
	gAppModule.RegisterBinder( m_pGameInputBinder, 999 );

	m_pUserInputBinder			= new InputBinder( "game" );
	gAppModule.RegisterBinder( m_pUserInputBinder, 1000 );

	m_pNisInputBinder		= new InputBinder( "nis" );
	gAppModule.RegisterBinder( m_pNisInputBinder, 1001 );

	PublishInterfaceToInputBinder();
	BindInputToPublishedInterface();

	m_RolloverObjData.object		= GOID_INVALID;
	m_RolloverObjData.pText			= 0;
	m_RolloverObjData.secondsTotal	= 0.0f;

	// Object selection indicators

	// Read the selection indicator colors out of a gas file
	FastFuelHandle handle( "config:global_settings" );
	FastFuelHandle hBlock = handle.GetChildNamed( "selection_indicators" );

	gpstring sTextureName;

	if ( hBlock.Get( "texture_default", sTextureName, false ) )
	{
		m_SelectionTextureDefault = gSiegeEngine.Renderer().CreateTexture( sTextureName, false, 0, TEX_LOCKED );
	} 
	else 
	{
		gperrorf(("Failed to load default selection circle texture from path: %s", sTextureName.c_str() ));
		m_SelectionTextureDefault = 0;
	}

	if ( hBlock.Get( "texture_focus", sTextureName, false ) )
	{
		m_SelectionTextureFocus = gSiegeEngine.Renderer().CreateTexture( sTextureName, false, 0, TEX_LOCKED );
	} 
	else 
	{
		gperrorf(("Failed to load default focus selection texture from path: %s", sTextureName.c_str() ));
		m_SelectionTextureFocus = 0;
	}

	gpstring sColor;

	if( hBlock.Get( "item_color", sColor ) ) 
	{
		if( stringtool::GetNumDelimitedStrings( sColor, ',' ) == 3 ) 
		{
			vector_3	color;
			stringtool::GetDelimitedValue( sColor, ',', 0, color.x );
			stringtool::GetDelimitedValue( sColor, ',', 1, color.y );
			stringtool::GetDelimitedValue( sColor, ',', 2, color.z );
			m_Item_selector_color = MAKEDWORDCOLOR( color );
		}
	} 
	else 
	{
		m_Item_selector_color				= 0xff00008f;
	}

	if ( hBlock.Get( "enemy_color", sColor ) ) {
		if ( stringtool::GetNumDelimitedStrings( sColor, ',' ) == 3 ) 
		{
			vector_3	color;
			stringtool::GetDelimitedValue( sColor, ',', 0, color.x );
			stringtool::GetDelimitedValue( sColor, ',', 1, color.y );
			stringtool::GetDelimitedValue( sColor, ',', 2, color.z );
			m_Enemy_selector_color = MAKEDWORDCOLOR( color );
		}
	} 
	else 
	{
		m_Enemy_selector_color				= 0xff8f0000;
	}

	if( hBlock.Get( "friend_color", sColor ) ) 
	{
		if( stringtool::GetNumDelimitedStrings( sColor, ',' ) == 3 ) 
		{
			vector_3	color;
			stringtool::GetDelimitedValue( sColor, ',', 0, color.x );
			stringtool::GetDelimitedValue( sColor, ',', 1, color.y );
			stringtool::GetDelimitedValue( sColor, ',', 2, color.z );
			m_Friend_selector_color = MAKEDWORDCOLOR( color );
		}
	} 
	else 
	{
		m_Friend_selector_color	= 0xff8f0000;
	}

	if ( hBlock.Get( "party_member_color", sColor ) ) {
		if ( stringtool::GetNumDelimitedStrings( sColor, ',' ) == 3 ) 
		{
			vector_3	color;
			stringtool::GetDelimitedValue( sColor, ',', 0, color.x );
			stringtool::GetDelimitedValue( sColor, ',', 1, color.y );
			stringtool::GetDelimitedValue( sColor, ',', 2, color.z );
			m_Party_member_selector_color = MAKEDWORDCOLOR( color );
		}
	} 
	else 
	{
		m_Party_member_selector_color		= 0xff008f00;
	}

	if(	hBlock.Get( "focus_go_color", sColor ) ) 
	{
		if( stringtool::GetNumDelimitedStrings( sColor, ',' ) == 3 )
		{
			vector_3	color;
			stringtool::GetDelimitedValue( sColor, ',', 0, color.x );
			stringtool::GetDelimitedValue( sColor, ',', 1, color.y );
			stringtool::GetDelimitedValue( sColor, ',', 2, color.z );
			m_Focus_go_selector_color = MAKEDWORDCOLOR( color );
		}
	} 
	else 
	{
		m_Focus_go_selector_color			= 0xff00ff00;
	}

	if ( hBlock.Get( "dead_enemy_member_color", sColor ) ) 
	{
		if( stringtool::GetNumDelimitedStrings( sColor, ',' ) == 3 ) 
		{
			vector_3	color;
			stringtool::GetDelimitedValue( sColor, ',', 0, color.x );
			stringtool::GetDelimitedValue( sColor, ',', 1, color.y );
			stringtool::GetDelimitedValue( sColor, ',', 2, color.z );
			m_Dead_enemy_selector_color = MAKEDWORDCOLOR( color );
		}
	}
	else 
	{
		m_Dead_enemy_selector_color			= 0xff000000;
	}

	if ( hBlock.Get( "dead_friend_member_color", sColor ) ) 
	{
		if( stringtool::GetNumDelimitedStrings( sColor, ',' ) == 3 ) 
		{
			vector_3	color;
			stringtool::GetDelimitedValue( sColor, ',', 0, color.x );
			stringtool::GetDelimitedValue( sColor, ',', 1, color.y );
			stringtool::GetDelimitedValue( sColor, ',', 2, color.z );
			m_Dead_friend_selector_color = MAKEDWORDCOLOR( color );
		}
	}
	else 
	{
		m_Dead_friend_selector_color			= 0xff000000;
	}


	DWORD dwPortraitInactiveColor = 0;
	FastFuelHandle hPortraitColors = handle.GetChildNamed( "portrait_colors" );
	if ( hPortraitColors.IsValid() && hPortraitColors.Get( "portrait_inactive_color", dwPortraitInactiveColor ) )
	{
		gUIPartyManager.SetPortraitInactiveColor( dwPortraitInactiveColor );
	}

	DWORD dwGridInvalidColor = 0;
	DWORD dwActiveSpellBookColor = 0;
	DWORD dwGridRollover = 0;
	DWORD dwGridRolloverOverlap = 0;
	DWORD dwGridMagicColor = 0;
		// LOA //
	DWORD dwGridSetItemColor = 0;
	DWORD dwGridImbuedColor = 0;
		// NEW //
	DWORD dwGridRareColor = 0;
	DWORD dwGridUniqueColor = 0;
	// DWORD dwGridCustomAColor = 0;
	// DWORD dwGridCustomBColor = 0;
	// DWORD dwGridCustomCColor = 0;
		// DOH //
	DWORD dwGridLegendaryColor = 0;
	DWORD dwGridArtifactColor = 0;
	
	FastFuelHandle hInvHighlights = handle.GetChildNamed( "inventory_highlights" );
	if ( hInvHighlights.IsValid() )
	{
		hInvHighlights.Get( "grid_invalid", dwGridInvalidColor );
		hInvHighlights.Get( "grid_active_spell_book", dwActiveSpellBookColor );
		hInvHighlights.Get( "grid_rollover", dwGridRollover );
		hInvHighlights.Get( "grid_rollover_overlap", dwGridRolloverOverlap );
		hInvHighlights.Get( "grid_rollover_overlap", dwGridRolloverOverlap );
		hInvHighlights.Get( "grid_magic", dwGridMagicColor );
			// LOA //
		hInvHighlights.Get( "grid_set_item"; dwGridSetItemColor );
		hInvHighlights.Get( "grid_imbued"; dwGridImbuedColor );
			// NEW //
		hInvHighlights.Get( "grid_rare"; dwGridRareColor );
		hInvHighlights.Get( "grid_unique"; dwGridUniqueColor );
		// hInvHighlights.Get( "grid_custom_a"; dwGridCustomAColor );
		// hInvHighlights.Get( "grid_custom_b"; dwGridCustombColor );
		// hInvHighlights.Get( "grid_custom_c"; dwGridCustomCColor );
			// DOH //
		hInvHighlights.Get( "grid_legendary"; dwGridLegendaryColor );
		hInvHighlights.Get( "grid_artifact"; dwGridArtifactColor );
		
		gUIInventoryManager.SetGridInvalidColor( dwGridInvalidColor );
		gUIInventoryManager.SetActiveBookColor( dwActiveSpellBookColor );
		gUIInventoryManager.SetGridRolloverColor( dwGridRollover );
		gUIInventoryManager.SetGridRolloverOverlapColor( dwGridRolloverOverlap );
		gUIInventoryManager.SetGridMagicColor( dwGridMagicColor );
			// LOA //
		gUIInventoryManager.SetGridSetItemColor( dwSetGridItemColor );
		gUIInventoryManager.SetGridImbuedColor( dwSetGridImbuedColor );
			// NEW //
		gUIInventoryManager.SetGridRareColor( dwSetGridRareColor );
		gUIInventoryManager.SetGridUniqueColor( dwSetGridRareColor );
			// DOH //
		gUIInventoryManager.SetGridLegendaryColor( dwSetGridLegendaryColor );
		gUIInventoryManager.SetGridArtifactColor( dwSetGridArtifactColor );
	}

	DWORD dwTextPositiveColor = 0;
	DWORD dwTextNegativeColor = 0;

	FastFuelHandle hCharHighlights = handle.GetChildNamed( "character_panel_highlights" );
	if ( hCharHighlights.IsValid() )
	{
		hCharHighlights.Get( "text_positive", dwTextPositiveColor );
		hCharHighlights.Get( "text_negative", dwTextNegativeColor );

		gUIPartyManager.SetPositiveTextColor( dwTextPositiveColor );
		gUIPartyManager.SetNegativeTextColor( dwTextNegativeColor );
	}

	const float SHORT_LEG		= -0.5f;
	const float LONG_LEG		=  1.5f;
	const float ABOVE_GROUND	=  0.125f;

	m_SelectionVerts[0].uv.u= 0;
	m_SelectionVerts[0].uv.v= 0;

	m_SelectionVerts[1].uv.u= 2;
	m_SelectionVerts[1].uv.v= 0;

	m_SelectionVerts[2].uv.u= 0;
	m_SelectionVerts[2].uv.v= 2;

	m_SelectionVerts[0].x = SHORT_LEG;
	m_SelectionVerts[0].z = SHORT_LEG;

	m_SelectionVerts[1].x = LONG_LEG;
	m_SelectionVerts[1].z = SHORT_LEG;

	m_SelectionVerts[2].x = SHORT_LEG;
	m_SelectionVerts[2].z = LONG_LEG;

	m_SelectionVerts[0].y =
	m_SelectionVerts[1].y =
	m_SelectionVerts[2].y = ABOVE_GROUND;

	FastFuelHandle hTipColors = handle.GetChildNamed( "tooltip_colors" );
	FastFuelFindHandle hFind( hTipColors );
	if ( hTipColors.IsValid() && hFind.IsValid() && hFind.FindFirstKeyAndValue() )
	{
		gpstring sKey, sValue;
		while ( hFind.GetNextKeyAndValue( sKey, sValue ) )
		{
			int color = 0;
			stringtool::Get( sValue, color );
			gUIShell.AddTipColor( sKey, color );
		}
	}

	// MP player labels
	m_pPlayerLabelText = new UIText;
	m_pPlayerLabelText->SetName( "mp_player_label" );
	m_pPlayerLabelText->SetAutoSize( true );
	m_pPlayerLabelText->SetJustification( JUSTIFY_CENTER );
	m_pPlayerLabelText->SetPassThrough( true );

	gpstring labelFont = "b_gui_fnt_12p_copperplate-light";
	FastFuelHandle hMpInGame( "config:multiplayer:ingame" );
	if ( hMpInGame.IsValid() )
	{
		labelFont = hMpInGame.GetString( "player_label_font", labelFont );
	}
	m_pPlayerLabelText->SetFont( labelFont );

	FastFuelHandle hMegaMap = handle.GetChildNamed( "mega_map" );
	if ( hMegaMap )
	{
		hMegaMap.Get( "show_border_meters", m_MapShowBorderMeters );
	}

	FastFuelHandle hLocale = handle.GetChildNamed( "locale" );
	if ( hLocale )
	{
		hLocale.Get( "resize_labels", m_bResizeLabels );
	}

	FastFuelHandle hMisc = handle.GetChildNamed( "gui_misc" );
	if ( hMisc )
	{
		int maxDisplay = 0;
		hMisc.Get( "max_generic_state_display", maxDisplay );
		gUIPartyManager.SetMaxGenericStateDisplay( maxDisplay );
	}
}