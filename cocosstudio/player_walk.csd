<GameProjectFile>
  <PropertyGroup Type="Node" Name="player_walk" ID="21b9bcc5-fc73-4b2f-89d8-adc14fc68a96" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="60" Speed="1.0000">
        <Timeline ActionTag="853645714" Property="FileData">
          <TextureFrame FrameIndex="0" Tween="False">
            <TextureFile Type="MarkedSubImage" Path="player_walk_1.png" Plist="Plist.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="30" Tween="False">
            <TextureFile Type="MarkedSubImage" Path="player_walk_2.png" Plist="Plist.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="60" Tween="False">
            <TextureFile Type="MarkedSubImage" Path="player_walk_1.png" Plist="Plist.plist" />
          </TextureFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Node" Tag="70" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="Sprite_1" ActionTag="853645714" Tag="71" IconVisible="False" ctype="SpriteObjectData">
            <Size X="65.0000" Y="78.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="player_walk_1.png" Plist="Plist.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>