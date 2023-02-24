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

	
