////////////////////////////////////////////////////////////////////////
// FETShape class definition
////////////////////////////////////////////////////////////////////////

struct FETShape {
public:
  // Constructor
  FETShape(FETReconstruction *reconstruction = NULL);
  FETShape(const FETShape& shape);
  ~FETShape(void);

  // Reconstruction access
  FETReconstruction *Reconstruction(void) const;
  int ReconstructionIndex(void) const;
  
  // Sequence access
  FETSequence *Sequence(void) const;
  int SequenceIndex(void) const;
  
  // Shape access
  int NParents(void) const;
  FETShape *Parent(int k) const;
  int NChildren(void) const;
  FETShape *Child(int k) const;

  // Feature access
  int NFeatures(void) const;
  FETFeature *Feature(int k) const;

  // Match access
  int NMatches(void) const;
  FETMatch *Match(int k) const;

  // Properties
  R3Box BBox(void) const;
  R3Point Centroid(void) const;
  R3Point Viewpoint(void) const;
  R3Vector Towards(void) const;
  R3Vector Up(void) const;
  RNLength AverageFeatureRadius(void) const;
  const R3Affine& Transformation(int transformation_type = 0) const;
  const char *Name(void) const;

  // Manipulation
  void ResetTransformation(void);
  void SetTransformation(const R3Affine& transformation);
  void PerturbTransformation(RNLength translation_magnitude, RNAngle rotation_magnitude);
  void SetInertia(RNScalar *variable_inertias, int nvariables = 9);
  void SetViewpoint(const R3Point& viewpoint);
  void SetTowards(const R3Vector& towards);
  void SetUp(const R3Vector& up);
  void SetName(const char *name);

  // Display
  void Draw(void) const;

  // Input/output
  int ReadAscii(FILE *fp);
  int ReadBinary(FILE *fp);
  int WriteAscii(FILE *fp) const;
  int WriteBinary(FILE *fp) const;

  // Feature search
  FETFeature *FindClosestFeature(const R3Point& query_position, 
    RNLength min_euclidean_distance = RN_UNKNOWN, RNLength max_euclidean_distance = RN_UNKNOWN);
  FETFeature *FindClosestFeature(FETFeature *query_feature, const R3Affine& query_transformation,
    RNLength min_euclidean_distance = RN_UNKNOWN, RNLength max_euclidean_distance = RN_UNKNOWN, 
    RNLength *max_descriptor_distances = NULL, RNAngle max_normal_angle = RN_UNKNOWN,
    RNScalar min_distinction = RN_UNKNOWN, RNScalar min_salience = RN_UNKNOWN,
    RNBoolean discard_boundaries = FALSE, RNBoolean opposite_facing_normals = FALSE);
  int FindAllFeatures(FETFeature *query_feature, const R3Affine& query_transformation, RNArray<FETFeature *>& result,
    RNLength min_euclidean_distance = RN_UNKNOWN, RNLength max_euclidean_distance = RN_UNKNOWN, 
    RNLength *max_descriptor_distances = NULL, RNAngle max_normal_angle = RN_UNKNOWN,
    RNScalar min_distinction = RN_UNKNOWN, RNScalar min_salience = RN_UNKNOWN,
    RNBoolean discard_boundaries = FALSE, RNBoolean opposite_facing_normals = FALSE);

public:
  // Internal properties
  R3Point Origin(void) const;
  void SetOrigin(const R3Point& origin);

  // Internal manipulation
  void InsertChild(FETShape *child);
  void RemoveChild(FETShape *child);
  void InsertFeature(FETFeature *feature);
  void RemoveFeature(FETFeature *feature);
  void InsertMatch(FETMatch *match, int k);
  void RemoveMatch(FETMatch *match, int k);
  void DeleteFeatures(void);

  // Internal transformation
  void Transform(R3Point& point) const;
  void Transform(R3Vector& vector) const;
  void InverseTransform(R3Point& point) const;
  void InverseTransform(R3Vector& vector) const;

  // Internal updates
  void UpdateFeatureProperties(void);
  void InvalidateBBox(void);
  void UpdateBBox(void);
  void InvalidateKdtree(void);
  void UpdateKdtree(void);

  // Variable stuff
  int NVariables(void) const;
  void UpdateVariableIndex(int& nvariables);
  void UpdateVariableValues(const RNScalar *x);

  // Paramerized transformations
  int ComputeTransformedPointCoordinates(const R3Point& position,
    RNAlgebraic *& px, RNAlgebraic *& py, RNAlgebraic *& pz) const;
  int ComputeTransformedVectorCoordinates(const R3Vector& vector,
    RNAlgebraic *& nx, RNAlgebraic *& ny, RNAlgebraic *& nz) const;

public:
  // Reconstruction
  FETReconstruction *reconstruction;
  int reconstruction_index;

  // Sequence
  FETSequence *sequence;
  int sequence_index;

  // Hierarchy
  RNArray<FETShape *> parents;
  RNArray<FETShape *> children;

  // Features and matches
  RNArray<FETFeature *> features;
  RNArray<FETMatch *> matches;

  // Transformation properties
  R3Affine initial_transformation;
  R3Affine current_transformation;
  R3Affine ground_truth_transformation;

  // Optimization properties
  static const int max_variables = 9;
  RNScalar variable_inertias[max_variables];
  int variable_index[max_variables];
  
  // Geometric properties
  R3Kdtree<struct FETFeature *> *kdtree;
  R3Point viewpoint; // untransformed
  R3Vector towards, up; // untransformed
  R3Box bbox; // transformed

  // Other properties
  R3Point origin; // untransformed
  char *name;
};



////////////////////////////////////////////////////////////////////////
// Transformation types
////////////////////////////////////////////////////////////////////////

enum {
  CURRENT_TRANSFORMATION,
  INITIAL_TRANSFORMATION,
  GROUND_TRUTH_TRANSFORMATION,
  NO_TRANSFORMATION,
  NUM_TRANSFORMATION_TYPES
};



////////////////////////////////////////////////////////////////////////
// Variable names
////////////////////////////////////////////////////////////////////////

enum {
  FET_TX, FET_TY, FET_TZ,
  FET_RX, FET_RY, FET_RZ,
  FET_SX, FET_SY, FET_SZ,
  FET_NUM_VARIABLES
};



////////////////////////////////////////////////////////////////////////
// Inline functions
////////////////////////////////////////////////////////////////////////

inline FETReconstruction *FETShape::
Reconstruction(void) const
{
  // Return reconstruction
  return reconstruction;
}



inline int FETShape::
ReconstructionIndex(void) const
{
  // Return index of this shape in reconstruction
  return reconstruction_index;
}



inline FETSequence *FETShape::
Sequence(void) const
{
  // Return sequence
  return sequence;
}



inline int FETShape::
SequenceIndex(void) const
{
  // Return index of this shape in sequence
  return sequence_index;
}



inline int FETShape::
NParents(void) const
{
  // Return number of parents
  return parents.NEntries();
}



inline FETShape *FETShape::
Parent(int k) const
{
  // Return parent in hierarchy
  return parents.Kth(k);
}



inline int FETShape::
NChildren(void) const
{
  // Return number of children
  return children.NEntries();
}



inline FETShape *FETShape::
Child(int k) const
{
  // Return kth child
  return children.Kth(k);
}



inline int FETShape::
NFeatures(void) const
{
  // Return number of features
  return features.NEntries();
}



inline FETFeature *FETShape::
Feature(int k) const
{
  // Return kth feature
  return features.Kth(k);
}



inline int FETShape::
NMatches(void) const
{
  // Return number of matchs
  return matches.NEntries();
}



inline FETMatch *FETShape::
Match(int k) const
{
  // Return kth match
  return matches.Kth(k);
}



inline R3Point FETShape::
Origin(void) const
{
  // Update origin
  if ((origin.X() == RN_UNKNOWN) && (origin.Y() == RN_UNKNOWN) && (origin.Z() == RN_UNKNOWN)) {
    ((FETShape *) this)->origin = Centroid();
  }
  
  // Return origin
  return origin;
}



inline R3Point FETShape::
Centroid(void) const
{
  // Return centroid
  if (BBox().IsEmpty()) return R3zero_point;
  else return BBox().Centroid();
}



inline R3Point FETShape::
Viewpoint(void) const
{
  // Return viewpoint
  R3Point result = viewpoint;
  result.Transform(current_transformation);
  return result;
}



inline R3Vector FETShape::
Towards(void) const
{
  // Return towards
  R3Vector result = towards;
  result.Transform(current_transformation);
  return result;
}



inline R3Vector FETShape::
Up(void) const
{
  // Return up
  R3Vector result = up;
  result.Transform(current_transformation);
  return result;
}



inline const R3Affine& FETShape::
Transformation(int transformation_type) const
{
  // Return transformation
  if (transformation_type == CURRENT_TRANSFORMATION) return current_transformation;
  else if (transformation_type == INITIAL_TRANSFORMATION) return initial_transformation;
  else if (transformation_type == GROUND_TRUTH_TRANSFORMATION) return ground_truth_transformation;
  else return R3identity_affine;
}



inline const char *FETShape::
Name(void) const
{
  // Return name
  return name;
}



inline void FETShape::
SetInertia(RNScalar *inertias, int ninertias)
{
  // Check if should make shape immovable
  if (!inertias) {
    // Set inertia weights to infinity
    for (int i = 0; i < max_variables; i++) {
      variable_inertias[i] = RN_INFINITY;
    }
  }
  else {    
    // Copy inertia weights
    ninertias = (ninertias < max_variables) ? ninertias : max_variables;
    for (int i = 0; i < ninertias; i++) {
      variable_inertias[i] = inertias[i];
    }
  }
}



inline void FETShape::
SetOrigin(const R3Point& origin)
{
  // Set origin
  this->origin = origin;
}



inline void FETShape::
SetViewpoint(const R3Point& viewpoint)
{
  // Set viewpoint
  this->viewpoint = viewpoint;
  this->viewpoint.InverseTransform(current_transformation);
}



inline void FETShape::
SetTowards(const R3Vector& towards)
{
  // Set towards
  this->towards = towards;
  this->towards.InverseTransform(current_transformation);
}



inline void FETShape::
SetUp(const R3Vector& up)
{
  // Set up
  this->up = up;
  this->up.InverseTransform(current_transformation);
}



inline void FETShape::
SetName(const char *name)
{
  // Set name
  if (this->name) free(this->name);
  if (name) this->name = strdup(name);
  else this->name = NULL;
}



inline void FETShape::
Transform(R3Point& point) const
{
  // Transform point
  point.Transform(current_transformation);
}



inline void FETShape::
Transform(R3Vector& vector) const
{
  // Transform vector
  vector.Transform(current_transformation);
}



inline void FETShape::
InverseTransform(R3Point& point) const
{
  // Inverse transform point
  point.InverseTransform(current_transformation);
}



inline void FETShape::
InverseTransform(R3Vector& vector) const
{
  // Inverse transform vector
  vector.InverseTransform(current_transformation);
}



inline int FETShape::
NVariables(void) const
{
  // Return the number of variables
  return max_variables;
}



