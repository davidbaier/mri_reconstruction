# MRI Reconstruction

The goal of this tool is to look for liason`s inside an Brain MRI and visualize them in 3D space. A second goal is to reconstruct them as 
good as possible.

ToDo:

- Load multiple MRI files
  - Extract informations from all files in a directory.
  - Place them correct in the 3D space. 
    - How to differ between vert. and horiz. images
    - Get distance between MRI shots.

- Extract Liasons positions & segmentations
  - Train CNN network
  - Implement inference 
  - Introduce Liasons extraction in processing pipeline

- Reconstruct the Liason through segmentation combination
  - (Idea) Use intersection points to restore Area between them



DISCLAIMER: The visualizations provided here tend not to be correct in any way. This projects intend is to learn Computer Vision (esp. segmentation) and basic rendering techniques.